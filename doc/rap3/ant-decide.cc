point_type
AntBase::decide() noexcept
{
  const point_type current_node = this->path.back();
  const auto free_space = this->count_free_space();
  size_t path_values[free_space];
  size_t total_value = 0;
  size_t j = 0;

  // Calcaulate weighted sum of unvisited neighbour nodes
  for (auto i = 0ull; i < this->matrix.size(); ++i) {
    if (this->tabu[i])
      continue;

    auto val = this->value(current_node, i);
    path_values[j] = val;
    total_value += val;
    ++j;
  }

  // Randomly select neightbour
  size_t random = rand() % total_value;
  random += 1;

  // Check range to which random value belongs to finally pick a path
  for (auto i = 0ull; i < free_space; ++i) {
    total_value -= path_values[i];
    if (random >= total_value)
      return find_nth_unvisited_(i + 1);
  }

}

