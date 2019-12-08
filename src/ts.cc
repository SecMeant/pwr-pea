#include "ts.hpp"

namespace pea {
  void
  tabu_list::add_tabu(entry_t e) noexcept
  {
    this->list.push_back(e);
  }

  void
  tabu_list::add_tabu(index_t v1, index_t v2) noexcept
  {
    this->list.push_back({v1, v2, max_penalty});
  }

  tabu_list::value_t
  tabu_list::get_tabu(index_t v1, index_t v2) noexcept
  {
    entry_t e(v1, v2);
    auto f = std::find(this->list.begin(), this->list.end(), e);
    if (f == this->list.end())
      return 0;
    return f->penalty;
  }

  void
  tabu_list::cycle() noexcept
  {
    std::remove_if(this->list.begin(), this->list.end(), [](entry_t &e) {
        return e.penalty == 1;
    });

    for (auto &e : this->list)
      --e.penalty;
  }

  void
  tabu_list::reset() noexcept
  {
    this->list.clear();
  }

  void
  tabu_list::display() const noexcept
  {
    for (auto &e : this->list)
      fmt::print("({},{},{}) | ", e.v1, e.v2, e.penalty);
    fmt::print("\n");
  }
} // namespace pea
