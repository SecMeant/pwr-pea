#pragma once
#include <vector>

#include "mst.hpp"
#include "path.hpp"
#include "tsp_params.hpp"
#include "utils.hpp"

#include "debug_print.h"

namespace pea {
  class tabu_list
  {
  public:
    using index_t = MSTMatrix::index_type;
    using value_t = MSTMatrix::value_type;

    static constexpr value_t max_penalty = 5;

    struct entry_t
    {
      index_t v1, v2;
      value_t penalty;

      entry_t(index_t v1, index_t v2, value_t penalty)
        : v1(v1)
        , v2(v2)
        , penalty(penalty)
      {}

      // Used to construct comperable object (for exmaple for std::find)
      entry_t(index_t v1, index_t v2)
        : v1(v1)
        , v2(v2)
      {}

      bool
      operator==(const entry_t &other) const noexcept
      {
        return this->v1 == other.v1 && this->v2 == other.v2;
      }
    };

    tabu_list() = delete;

    tabu_list(size_t size)
      : list(size)
    {
      std::fill(this->list.begin(), this->list.end(), 0);
    }

    using list_t = std::vector<entry_t>;

    void
    add_tabu(entry_t e) noexcept;

    void
    add_tabu(index_t v1, index_t v2) noexcept;

    value_t
    get_tabu(index_t v1, index_t v2) noexcept;

    void
    cycle() noexcept;

    void
    reset() noexcept;

    void
    display() const noexcept;

  private:
    MSTMatrix list;
  };

  template<typename SwapProcType, init_strat_e InitStrat>
  class tabu : private SwapProcType
  {
  public:
    using index_t = MSTMatrix::index_type;
    using value_t = MSTMatrix::value_type;

    static constexpr auto init_strat = InitStrat;

    static constexpr MSTMatrix::index_type start_node = 0;

    using try_swap_res = typename SwapProcType::try_swap_res;

    tabu() = default;

    template<typename MatrixType>
    inline tabu(MatrixType &&m)
      : matrix(std::forward<MatrixType>(m))
      , list(matrix.size())
    {}

    tabu(const tabu &t) = default;
    tabu(tabu &&t) = default;

    tabu &
    operator=(const tabu &t) = default;

    tabu &
    operator=(tabu &&t) = default;

    ~tabu() = default;

    // TODO this should be private
  public:
    void
    swap(MSTMatrix::iterator a, MSTMatrix::iterator b) noexcept
    {
      this->swapper(a, b);
    }

    void
    reset()
    {
      if (this->matrix.size() <= 2)
        return;

      solved = false;

      if constexpr (init_strat == init_strat_e::trivial) {

        this->current_path = Path::generate_simple(this->matrix.size());
        this->optimal_path = this->current_path;
        this->cost = pea::cost(this->matrix, this->optimal_path);

      } else if constexpr (init_strat == init_strat_e::nearest_neighbour) {

        // Build MST
        this->current_path.clear();
        this->current_path.push_back(start_node);
        this->cost = 0;
        while (this->current_path.size() < this->matrix.size()) {

          // Look for neightbour that is the cheapest
          // and not yet in the path
          auto pred = [this](MSTMatrix::index_type idx) -> bool {
            auto f = std::find(
                this->current_path.begin(), this->current_path.end(), idx);
            return f == this->current_path.end();
          };

          auto next_node =
            matrix.nearest_neighbour_if(current_path.back(), pred);

          assert(next_node.v2 != -1);

          this->current_path.push_back(next_node.v2);
        }

        this->optimal_path = this->current_path;
        this->cost = pea::cost(this->matrix, this->optimal_path);
      } else if constexpr (init_strat == init_strat_e::random) {
        this->current_path = Path::generate_random(this->matrix.size());
        this->optimal_path = this->current_path;
        this->cost = pea::cost(this->matrix, this->optimal_path);
      }

    }

    try_swap_res
    try_swap(index_t v1_idx, index_t v2_idx) noexcept
    {
      return this->swapper.try_swap(this->current_path.it_at(v1_idx),
                                    this->current_path.it_at(v2_idx));
    }

    void
    swap(index_t v1_idx, index_t v2_idx) noexcept
    {
      this->swapper(this->current_path.it_at(v1_idx),
                    this->current_path.it_at(v2_idx));
    }

    void
    display() const noexcept
    {
      this->current_path.display();
      this->optimal_path.display();
      this->list.display();
      fmt::print("Cost: {}\n", this->cost);
    }

    struct swap_candidate
    {
      index_t v1, v2;
      cost_t cost;
    };

    Path
    solve() noexcept
    {
      if (solved)
        return this->optimal_path;

      auto no_swap_cycles = 0;
      constexpr auto no_swap_breakdown = 500;
      auto cycle_count = 100'000;
      this->reset();
      this->list.reset();
      do {
        swap_candidate best_swap{};
        best_swap.cost = cost_inf;

        for (index_t i = 0; i < this->matrix.size(); i++) {
          for (index_t j = i + 1; j < this->matrix.size(); j++) {

            auto new_cost = this->try_swap(i, j);

            if (new_cost.amor_cost < best_swap.cost)
              best_swap = { i, j, new_cost.cost };
          }
        }

        this->swap(best_swap.v1, best_swap.v2);
        this->list.add_tabu(best_swap.v1, best_swap.v2);

        ++no_swap_cycles;

        if (best_swap.cost < this->cost) {
          this->optimal_path = this->current_path;
          this->cost = pea::cost(this->matrix, this->optimal_path);
          no_swap_cycles = 0;
        }

        this->list.cycle();
        cycle_count--;

        if (no_swap_cycles == no_swap_breakdown) {
          no_swap_cycles = 0;
          this->current_path = Path::generate_random(this->matrix.size());
        }
      } while (cycle_count >= 0);

      solved = true;
      return this->optimal_path;
    }

    auto
    get_cost()
    {
      return this->cost;
    }

    friend SwapProcType;

  private:
    MSTMatrix matrix;
    Path current_path;
    Path optimal_path;
    cost_t cost;
    tabu_list list;
    SwapProcType swapper;
    bool solved = false;
  };

  template<template<init_strat_e> class SwapProcType, init_strat_e InitStrat>
  using tabu_solver = tabu<SwapProcType<InitStrat>, InitStrat>;

} // namespace pea
