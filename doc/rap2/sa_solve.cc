Path
solve()
{
  const auto max_index = this->matrix.size() - 1;
  auto temperature = initial_temperature;
  auto current_cost = cost_inf;
  auto time = 0;
  this->reset();
  do {
    auto i = random_index(0, max_index);
    auto j = random_index(0, max_index);

    auto newcost = this->try_swap(i, j);
    auto force_swap = temp_swap(temperature, newcost, current_cost);

    if (newcost < current_cost || force_swap) {
      this->swap(i, j);
      current_cost = pea::cost(this->matrix, this->current_path);
    }

    if (current_cost < this->cost) {
      this->optimal_path = this->current_path;
      this->cost = current_cost;
    }

    regulate_temperature(temperature, time);
    ++time;

  } while (temperature > end_temperature);

  return this->optimal_path;
}
