#pragma once

#include "ant.h"
#include "mst.hpp"
#include "path.hpp"
#include "utils.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <utility>

namespace pea::aco {
  constexpr size_t DefaultColonySize = 1000;

  template<size_t ColonySize = DefaultColonySize>
  class solver
  {
  public:
    static constexpr size_t colony_size = ColonySize;
    static constexpr size_t max_iter = 500;
    static constexpr size_t start_pheromones = 700;
    static constexpr size_t evaporation_coeff = 2;

    // Amount of pheromone that evaporates each cycle
    static constexpr size_t phro_evaporate_amnt = 1;

    template<typename MatrixType>
    inline solver(MatrixType &&m)
      : matrix(std::forward<MatrixType>(m))
      , pheromones(m.size())
    {}

    ~solver() = default;

  public:
    void
    reset()
    {
      const auto node_count = this->matrix.size();

      if (node_count <= 2)
        return;

      this->pheromones.resize(node_count);
      for (auto i = 0ull; i < node_count; ++i)
        for (auto j = 0ull; j < node_count; ++j)
          this->pheromones.set(j, i, start_pheromones);

      this->ants.clear();
      for (auto i = 0ull; i < colony_size; ++i) {
        this->ants.emplace_back(this->matrix, this->pheromones);
        point_type random_node = rand() % this->matrix.size();
        this->ants[i].reset(random_node);
      }

      this->best = {};
      this->cost = cost_inf;
    }

    void
    evaporate() noexcept
    {
      static constexpr auto min_phro = AntBase::phro_place_amnt / 8;
      for (auto & ph : this->pheromones) {
        ph -= phro_evaporate_amnt;
        if (ph <= min_phro)
          ph = min_phro;
      }
    }

    Path
    solve() noexcept
    {
      this->reset();

      size_t node_count = this->matrix.size();

      for (auto i = 0ull; i < max_iter; ++i) {
        for (auto j = 1ull; j < node_count; ++j) {
          for (auto &ant : this->ants) {
            ant.move();
          }
          this->evaporate();
        }

        for (auto &ant : this->ants) {

          if (ant.get_cost() < this->cost) {
            this->best = ant.steal_path_();
            this->cost = ant.get_cost();
            fmt::print("new best cost {}\n", this->cost);
          }

          ant.reset(rand() % node_count);
        }
      }

      return this->best;
    }

    const auto &
    get_matrix()
    {
      return this->matrix;
    }

  private:
    MSTMatrix matrix;
    MSTMatrix pheromones;
    std::vector<Ant<AcoType::QAS>> ants;
    Path best;
    cost_t cost;
  };

} // namespace pea::aco
