template<>
inline void
Ant<AcoType::DAS>::move() noexcept
{
  point_type current_node = this->path.back();

  // decide() ensures that next_node will be unique in scope of the existing path
  point_type next_node = this->decide(); 

  this->cost += this->matrix.get(current_node, next_node);
  this->path.push_back(next_node);
  this->tabu[next_node] = true;
  this->phro.set(current_node, next_node, phro_place_amnt);

  if (this->path.size() == this->matrix.size())
    this->cost += this->matrix.get(next_node, this->path.front());
};

template<>
inline void
Ant<AcoType::QAS>::move() noexcept
{
  point_type current_node = this->path.back();

  // decide() ensures that next_node will be unique in scope of the existing path
  point_type next_node = this->decide();

  auto edge_cost = this->matrix.get(current_node, next_node);

  if (edge_cost == 0)
    edge_cost = 1;

  this->cost += edge_cost;
  this->path.push_back(next_node);
  this->tabu[next_node] = true;

  auto old_phro = this->phro.get(current_node, next_node);
  auto new_phro = old_phro + phro_place_amnt / edge_cost;
  this->phro.set(current_node, next_node, new_phro);

  if (this->path.size() == this->matrix.size())
    this->cost += this->matrix.get(next_node, this->path.front());
};
