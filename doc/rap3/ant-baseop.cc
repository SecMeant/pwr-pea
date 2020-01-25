void
AntBase::reset(point_type new_start_node) noexcept
{
  auto tabu_begin = this->tabu.get();
  auto tabu_end = tabu_begin + this->matrix.size();

  std::fill(tabu_begin, tabu_end, false);

  this->cost = 0;

  this->path.clear();

  this->path.push_back(new_start_node);
  this->tabu[new_start_node] = true;
}

cost_t
AntBase::value(point_type from, point_type to) noexcept
{
  auto path_cost = this->matrix.get(from, to);
  size_t phero_amnt = this->phro.get(from, to);

  // For zero cost path return some arbitrary high value
  if (path_cost <= 0)
    return 20000;

  auto value = (phero_amnt * pvcoeff) / static_cast<size_t>(path_cost);

  return value + 1; // No value 0 is allowed
}
