#pragma once
#include <vector>

#include "mst.hpp"
#include "path.hpp"
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
    list_t list;
  };


  template<typename SwapProcType>
  class tabu
    : private SwapProcType
  {
  public:
    using index_t = MSTMatrix::index_type;
    using value_t = MSTMatrix::value_type;

    static constexpr MSTMatrix::index_type start_node = 0;

    using try_swap_res = typename SwapProcType::try_swap_res;

    tabu() = default;

    template<typename MatrixType>
    inline tabu(MatrixType &&m)
      : matrix(std::forward<MatrixType>(m))
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
      this->current_path = Path::generate_simple(this->matrix.size());
      this->optimal_path = Path::generate_simple(this->matrix.size());
      this->cost = pea::cost(this->matrix, this->optimal_path);
      return;

      if (this->matrix.size() <= 2)
        return;

      solved = false;

      // Build MST
      this->current_path.clear();
      this->current_path.push_back(start_node);
      this->cost = 0;
      while (this->current_path.size() < this->matrix.size()) {

        // Look for neightbour that is the cheapest and not yet in the path
        auto pred = [this](MSTMatrix::index_type idx) -> bool {
          auto f = std::find(this->current_path.begin(), this->current_path.end(), idx);
          return f == this->current_path.end();
        };

        auto next_node = matrix.nearest_neighbour_if(current_path.back(), pred);

        assert(next_node.v2 != -1);

        this->current_path.push_back(next_node.v2);
      }

      this->optimal_path = this->current_path;
      this->cost = pea::cost(this->matrix, this->optimal_path);
    }

    inline value_t
    wrap_matrix_get_(index_t v1, index_t v2) const noexcept FORCE_INLINE
    {
      value_t cost;
      if (v1 >= this->matrix.size() || v1 < 0)
        cost = 0;

      if (v2 >= this->matrix.size() || v2 < 0)
       cost = 0;

      cost = this->matrix.get(v1, v2);
      return cost;
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
      this->swapper(this->optimal_path.it_at(v1_idx), this->optimal_path.it_at(v2_idx));
      this->swapper(this->current_path.it_at(v1_idx), this->current_path.it_at(v2_idx));
      this->cost = pea::cost(this->matrix, this->optimal_path);
    }

    void display() const noexcept
    {
      this->current_path.display();
      this->optimal_path.display();
      this->list.display();
      fmt::print("Cost: {}\n", this->cost);
    }

    Path solve() noexcept
    {
      if (solved)
        return this->optimal_path;

      auto asdf = 10'000'0;
      this->reset();
      this->list.reset();
              this->display();
      do
      {
        for (index_t i = 0; i < this->matrix.size(); i++) {
          for (index_t j = i; j < this->matrix.size(); j++) {

            auto new_cost = this->try_swap(i,j);

            if (new_cost.amor_cost < this->cost) {
              fmt::print("Swap beneficial\n");
              this->display();
              this->swap(i,j);
              this->list.add_tabu(i,j);
            }

          }
        }

        this->list.cycle();
        asdf--;
      }while(asdf >= 0);

      solved = true;
      return this->optimal_path;
    }

    auto get_cost()
    {return this->cost;}

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
} // namespace pea
