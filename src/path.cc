#include "path.hpp"
#include "mst.hpp"
#include "typealias.hpp"

namespace pea {
  cost_t
  cost(const MSTMatrix &matrix, const Path &path) noexcept
  {
    auto f = path.begin();
    auto n = f + 1;
    const auto end = path.end();
    cost_t cost = 0;

    while (n != end) {
      auto tmp_cost = matrix.get(*f, *n);

      assert(tmp_cost > 0);

      cost += static_cast<cost_t>(tmp_cost);

      ++f;
      ++n;
    }

    cost += static_cast<cost_t>(matrix.get(*f, path[0]));
    return cost;
  }
} // namespace pea
