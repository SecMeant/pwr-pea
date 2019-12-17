template<init_strat_e strat>
struct invert_op
{
  using parent_t = tabu<invert_op, strat>;

  void
  operator()(Path::iterator a, Path::iterator b)
  {
    this->swap(a, b);
  }

  void
  swap(Path::iterator a, Path::iterator b)
  {
    std::reverse(a, std::next(b));
  }

  struct try_swap_res
  {
    cost_t cost;
    cost_t amor_cost;
  };

  try_swap_res
  try_swap(Path::iterator a, Path::iterator b)
  {
    auto parent = container_of(this, parent_t, swapper);
    try_swap_res res;

    this->swap(a, b);
    res.cost = pea::cost(parent->matrix, parent->current_path);
    res.amor_cost = res.cost;
    this->swap(a, b);

    return res;
  }
};
