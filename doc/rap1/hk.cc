#include "combgen.hpp"
#include "hk.hpp"
#include <fmt/format.h>

namespace pea {

  static Path
  hk_find_optimal_path(const MSTMatrix &matrix, MemoTable &memo) noexcept
  {
    constexpr point_type start_node = 0;
    auto node_count = matrix.size();
    std::bitset<max_problem_size> state = (1 << node_count) - 1;
    Path path;
    path.resize(matrix.size());
    size_t last = start_node;

    for (size_t i = node_count - 1; i >= 1; --i) {

      cost_t minDist = cost_inf;
      size_t end_node = start_node;

      for (size_t e = 0; e < node_count; ++e) {

        if (!hk_isset(e, state) || e == start_node)
          continue;

        cost_t newDist = memo.get(e, state.to_ulong());
        newDist += matrix.get(e, last);

        if (newDist < minDist) {
          minDist = newDist;
          end_node = e;
        }
      }

      last = end_node;
      path[i] = end_node;
      state.reset(end_node);
    }

    return path;
  }

  static Path
  hk_solve(const MSTMatrix &matrix, MemoTable &memo)
  {
    constexpr point_type start_node = 0;

    // Because implementation uses bitset it requires it.
    // std::vector cen be used, but lets be honest --
    // bitset is much better than std::vector<bool>,
    // the drawback is the static aspect of it ;/
    assert(matrix.size() <= max_problem_size);
    if (matrix.size() > max_problem_size)
      return {};

    auto node_count = matrix.size();

    for (size_t r = 3; r <= node_count; ++r) {
      for (auto &subset : comb<max_problem_size>(r, node_count)) {
        if (!hk_isset(start_node, subset))
          continue;
        for (size_t next = 0; next < node_count; ++next) {

          if (next == start_node || !hk_isset(next, subset))
            continue;

          std::bitset<max_problem_size> state = subset;
          state.reset(next);

          cost_t minDist = cost_inf;

          for (size_t e = 0; e < node_count; ++e) {
            if (e == start_node || e == next || !hk_isset(e, subset))
              continue;

            cost_t newDist =
              memo.get(e, state.to_ulong()) + matrix.get(e, next);

            if (newDist < minDist)
              minDist = newDist;

          }

          memo.set(next, subset.to_ulong(), minDist);
        }
      }
    }

    return hk_find_optimal_path(matrix, memo);
  }

  Path
  hksolve(const MSTMatrix &matrix)
  {
    auto node_count = matrix.size();
    MemoTable memo(node_count, (1 << node_count));

    hk_setup(matrix, memo);
    Path p = hk_solve(matrix, memo);

    return p;
  }
} // namespace pea
