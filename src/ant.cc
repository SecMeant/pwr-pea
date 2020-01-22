#include "ant.h"

#include <algorithm>

namespace pea {

  void
  AntBase::reset(point_type new_start_node) noexcept
  {
    auto tabu_begin = this->tabu.get();
    auto tabu_end = tabu_begin + this->matrix.size();
    std::fill(tabu_begin, tabu_end, false);

    this->cost = 0;

    this->path.clear();

    this->path.push_back(new_start_node);
    this->tabu[new_start_node] = true;
  }

  UNSAFE point_type AntBase::find_nth_unvisited_(size_t nth) const noexcept
  {
    for (point_type i = 0ull; i < this->matrix.size(); ++i) {
      if (!this->tabu[i]) {
        --nth;
        if (nth == 0)
          return i;
      }
    }

    CANT_TOUCH_THIS;
  }

  point_type
  AntBase::decide() noexcept
  {
    const point_type current_node = this->path.back();
    const auto free_space = this->count_free_space();
    size_t path_values[free_space];
    size_t total_value = 0;
    size_t j = 0;

    BUGON(free_space <= 0);

    // Calcaulate weighted sum of unvisited neighbour nodes
    for (auto i = 0ull; i < this->matrix.size(); ++i) {
      if (this->tabu[i])
        continue;

      auto val = this->value(current_node, i);
      path_values[j] = val;
      total_value += val;
      ++j;
    }

    // Randomly select neightbour
    size_t random = rand() % total_value;
    random += 1;

    // Check range to which random value belongs to finally pick a path
    for (auto i = 0ull; i < free_space; ++i) {
      total_value -= path_values[i];
      if (random >= total_value)
        return find_nth_unvisited_(i+1);
    }

    CANT_TOUCH_THIS;
  }

  cost_t
  AntBase::value(point_type from, point_type to) noexcept
  {
    auto path_cost = this->matrix.get(from, to);
    size_t phero_amnt = this->phro.get(from, to);

    BUGON(phero_amnt < 0);

    if (path_cost <= 0)
      return 20000;

    auto value = (phero_amnt * pvcoeff) / static_cast<size_t>(path_cost);
    return value + 1; // No value 0 is allowed
  }

} // namespace pea
