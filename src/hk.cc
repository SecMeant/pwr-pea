#include "combgen.hpp"
#include "hk.hpp"
#include <fmt/format.h>

namespace pea {

  constexpr auto max_problem_size = 32;

  // Generates first sub paths -- from 0th node to all the rest
  static void
  hk_setup(const MSTMatrix &matrix, MemoTable &memo)
  {
    for (size_t i = 1; i < matrix.size(); ++i)
      memo.set(i, (1 << i) | 1, matrix.get(0, i));
  }

  template<typename bitset_type>
  bool
  hk_isset(size_t i, const bitset_type &b)
  {
    return b[i];
  }

  cost_t
  hk_find_best_cost(const MSTMatrix &matrix, MemoTable &memo)
  {
    constexpr point_type start_node = 0;
    auto node_count = matrix.size();

    std::bitset<max_problem_size> endstate((1 << node_count) - 1);
    cost_t minDist = cost_inf;

    for (size_t e = 0; e < node_count; ++e) {
      if (e == start_node)
        continue;

      cost_t newDist = memo.get(e, endstate.to_ulong()) + matrix.get(e, start_node);
      if (newDist < minDist)
        minDist = newDist;
    }

    return minDist;
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
        for (size_t next = 0; next < node_count; ++next) {

          if (next == start_node || !hk_isset(next, subset))
            continue;

          std::bitset<max_problem_size> state = subset;
          state.reset(next);

          cost_t minDist = cost_inf;

          for (size_t e = 0; e < node_count; ++e) {
            if (e == start_node || e == next || !hk_isset(e, subset))
              continue;

            cost_t newDist = memo.get(e, state.to_ulong()) + matrix.get(e, next);

            if (newDist < minDist)
              minDist = newDist;

            memo.set(next, subset.to_ulong(), minDist);
          }
        }
      }
    }

    return {hk_find_best_cost(matrix, memo)};
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
