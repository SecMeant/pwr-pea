#include "ts.hpp"

namespace pea {
  void
  tabu_list::add_tabu(index_t v1, index_t v2) noexcept
  {
    auto penalty = this->current_cycle + this->max_penalty;
    this->list.set(v1, v2, penalty);
    this->list.set(v2, v1, penalty);
  }

  tabu_list::value_t
  tabu_list::get_tabu(index_t v1, index_t v2) noexcept
  {
    auto tabu = this->list.get(v1, v2);

    if (tabu < this->current_cycle)
      return 0;

    return tabu - this->current_cycle;
  }

  void
  tabu_list::cycle() noexcept
  {
    this->current_cycle++;
  }

  void
  tabu_list::reset() noexcept
  {
    this->current_cycle = 0;
    for (auto &e : this->list)
      e = 0;
  }

  void
  tabu_list::display() const noexcept
  {
    for (auto i = 0u; i < this->list.size(); ++i) {
      for (auto j = 0u; j < this->list.size(); ++j) {
        auto pen = this->list.get(i,j);
        if (pen)
          fmt::print("({},{},{}) | ", i, j, pen);
      }
    }
    fmt::print("\n");
  }
} // namespace pea
