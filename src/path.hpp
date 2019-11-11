#pragma once
#include <limits>

#include "typealias.hpp"
#include "mst.hpp"

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
  };

  constexpr cost_t cost_inf = std::numeric_limits<cost_t>::max();

  cost_t
  cost(const MSTMatrix &matrix, const Path &path) noexcept;

} // namespace pea
