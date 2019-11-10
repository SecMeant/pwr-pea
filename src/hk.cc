#include "hk.hpp"
#include "combgen.hpp"

namespace pea {

  // Generates first sub paths -- from 0th node to all the rest
  static void
  hk_setup(const MSTMatrix &matrix, MemoTable &memo)
  {
    for (size_t i = 1; i < matrix.size(); ++i)
      memo.set(i, (1 << i) | 1, matrix.get(0, i));
  }

  template<typename bitset_type>
  bool hk_isset(size_t i, const bitset_type &b)
  { return b[i]; }

  static Path
  hk_solve(const MSTMatrix &matrix, MemoTable &memo)
  {
    constexpr auto max_problem_size = 32;

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
        
      }
    }
  }

  Path
  hksolve(const MSTMatrix &matrix)
  {
    constexpr point_type start_node = 0;
    auto node_count = matrix.size;
    MemoTable memo(node_count, (1 << node_count));

    hk_setup(matrix, memo);
    Path p = hk_solve(matrix, memo);
  }
} // namespace pea
