#pragma once
#include <limits>
#include <numeric>

#include <cstdlib>

#include "mst.hpp"
#include "typealias.hpp"

namespace pea {

  using point_type = size_t;

  class Path : public std::vector<point_type>
  {
  public:
    Path(std::initializer_list<point_type> il) noexcept
      : std::vector<point_type>(il)
    {}

    template<typename... Args>
    Path(Args &&... args) noexcept
      : std::vector<point_type>(std::forward<Args>(args)...)
    {}

    static Path
    generate_simple(size_t node_count) noexcept
    {
      std::vector<point_type> v(node_count);
      std::iota(v.begin(), v.end(), 0);
      return v;
    }

    static Path
    generate_random(size_t node_count) noexcept
    {
      std::vector<point_type> v = generate_simple(node_count);
      for (auto i = 0u; i < node_count; ++i)
        std::swap(v[rand()%node_count], v[rand()%node_count]);
      return v;
    }

    void
    display() const noexcept
    {
      for (auto &p : *this)
        fmt::print("{} -> ", p);
      fmt::print("\n");
    }

    iterator it_at(size_type idx)
    {
      return this->begin() + idx;
    }
  };

  constexpr cost_t cost_inf = std::numeric_limits<cost_t>::max();

  cost_t
  cost(const MSTMatrix &matrix, const Path &path) noexcept;

} // namespace pea
