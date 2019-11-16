namespace pea {
  Path
  bt(const MSTMatrix &matrix, cost_t *costp)
  {
    Path p = Path::generate_simple(matrix.size());
    Path res;
    cost_t rescost = std::numeric_limits<decltype(rescost)>::max();

    do {
      cost_t tmp_cost = cost(matrix, p);
      if (tmp_cost < rescost) {
        rescost = tmp_cost;
        res = p;
      }
    } while (::next_permutation(std::begin(p), std::end(p)));

    if (costp)
      *costp = rescost;

    return res;
  }
} // namespace pea
