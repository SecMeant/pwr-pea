class tabu_list
{
public:
  using index_t = MSTMatrix::index_type;
  using value_t = MSTMatrix::value_type;

  static constexpr value_t max_penalty = 5;

  add_tabu(index_t v1, index_t v2) noexcept
  {
    auto penalty = this->current_cycle + this->max_penalty;
    this->list.set(v1, v2, penalty);
    this->list.set(v2, v1, penalty);
  }

  value_t
  get_tabu(index_t v1, index_t v2) noexcept
  {
    auto tabu = this->list.get(v1, v2);

    if (tabu < this->current_cycle)
      return 0;

    return tabu - this->current_cycle;
  }

  void
  cycle() noexcept
  {
    this->current_cycle++;
  }

private:
  MSTMatrix list;
  value_t current_cycle;
};
