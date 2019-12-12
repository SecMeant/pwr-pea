#pragma once

#include "mst.hpp"
#include "path.hpp"
#include "tsp_params.hpp"
#include "utils.hpp"

namespace pea {

  enum class SwapProcType
  {
    swap,
    insert,
    invert
  };

  enum class TempProcType
  {
    standard,
    log,
    geometric
  };

  template<SwapProcType sproc, TempProcType tproc, init_strat_e InitStrat>
  class simulated_annealing
  {
  public:
    static constexpr auto init_strat = InitStrat;

    using index_t = MSTMatrix::index_type;
    using value_t = MSTMatrix::value_type;

    static constexpr MSTMatrix::index_type start_node = 0;
    static constexpr float initial_temperature = 300;
    static constexpr float end_temperature = 0;
    static constexpr float cooling_rate = 0.2;

    template<typename MatrixType>
    inline simulated_annealing(MatrixType &&m)
      : matrix(std::forward<MatrixType>(m))
    {}

    ~simulated_annealing() = default;

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

    cost_t
    try_swap(index_t a, index_t b) noexcept
    {
      return try_swap(this->current_path.it_at(a),
                      this->current_path.it_at(b));
    }

    cost_t
    try_swap(Path::iterator a, Path::iterator b) noexcept
    {
      this->swap(a, b);
      auto res = pea::cost(this->matrix, this->current_path);
      this->swap(a, b);

      return res;
    }

    void
    swap(index_t a, index_t b) noexcept
    {
      swap(this->current_path.it_at(a), this->current_path.it_at(b));
    }

    void
    swap(Path::iterator a, Path::iterator b) noexcept
    {
      if constexpr (sproc == SwapProcType::swap) {

        std::iter_swap(a, b);

      } else if constexpr (sproc == SwapProcType::insert) {

        if (a < b) {

          auto copy = std::move(*b);
          auto it = b;

          while (it != a) {
            *it = std::move(*(it - 1));
            std::advance(it, -1);
          }

          *a = std::move(copy);

        } else {

          auto copy = std::move(*b);
          auto it = b;

          while (it != a) {
            *it = std::move(*(it + 1));
            std::advance(it, 1);
          }

          *a = std::move(copy);
        }

      } else {

        std::reverse(a, std::next(b));
      }
    }

    void
    display() const noexcept
    {
      this->current_path.display();
      this->optimal_path.display();
      this->list.display();
      fmt::print("Cost: {}\n", this->cost);
    }

    inline static double
    calc_prob(double temperature,
              double newcost,
              double oldcost) noexcept FORCE_INLINE
    {
      return std::exp(-(newcost - oldcost) / temperature);
    }

    inline static bool
    temp_swap(double temperature,
              double newcost,
              double oldcost) noexcept FORCE_INLINE
    {
      auto p = static_cast<double>(rand() % 100) / 100.0;
      auto prob = calc_prob(temperature, newcost, oldcost);
      return p < prob;
    }

    inline static void
    regulate_temperature(float &temp, size_t time) noexcept FORCE_INLINE
    {
      if constexpr (tproc == TempProcType::standard) {

        temp = initial_temperature - (cooling_rate * time);

      } else if constexpr (tproc == TempProcType::log) {

        auto temp_copy = temp;
        temp = initial_temperature / std::log(time+1);
        if (temp == temp_copy)
          temp = end_temperature;

      } else if constexpr (tproc == TempProcType::geometric) {

        temp = initial_temperature * std::pow(cooling_rate, time);
      }
    }

    size_t
    random_index(size_t begin_range, size_t end_range)
    {
      auto range = end_range - begin_range;
      return (rand() % range) + begin_range;
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

      const auto max_index = this->matrix.size() - 1;
      auto temperature = initial_temperature;
      auto current_cost = cost_inf;
      auto time = 0;
      auto qwer = 0;
      this->reset();
      do {
        auto i = random_index(0, max_index);
        auto j = random_index(0, max_index);

        auto newcost = this->try_swap(i, j);
        auto force_swap = temp_swap(temperature, newcost, current_cost);

        qwer++;
        //fmt::print("{}\n", qwer);

        if (newcost < current_cost || force_swap) {
          //fmt::print("New best cost: {} < {}\n", newcost, current_cost);
          this->swap(i, j);
          current_cost = pea::cost(this->matrix, this->current_path);
        }

        if (current_cost < this->cost) {
          this->optimal_path = this->current_path;
          this->cost = current_cost;
          qwer = 0;
          //fmt::print("better\n", qwer);
        }

        regulate_temperature(temperature, time);
        ++time;

      } while (temperature > end_temperature);

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
    bool solved = false;
  };

  template<SwapProcType sproc, TempProcType tproc, init_strat_e initstrat>
  using simann = simulated_annealing<sproc, tproc, initstrat>;

} // namespace pea
