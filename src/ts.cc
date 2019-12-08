#include "ts.hpp"

namespace pea {
  void
  tabu_list::add_tabu(entry_t e) noexcept
  {
    this->list.set(e.v1, e.v2, e.penalty);
  }

  void
  tabu_list::add_tabu(index_t v1, index_t v2) noexcept
  {
    this->list.set(v1, v2, this->max_penalty);
  }

  tabu_list::value_t
  tabu_list::get_tabu(index_t v1, index_t v2) noexcept
  {
    return this->list.get(v1, v2);
  }

  void
  tabu_list::cycle() noexcept
  {
    for (auto &e : this->list)
      if (e > 0) --e;
  }

  void
  tabu_list::reset() noexcept
  {
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
