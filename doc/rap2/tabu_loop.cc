template<typename SwapProcType, init_strat_e InitStrat>
class tabu : private SwapProcType
{
public:
  using index_t = MSTMatrix::index_type;
  using value_t = MSTMatrix::value_type;
  static constexpr auto init_strat = InitStrat;
  static constexpr MSTMatrix::index_type start_node = 0;
  using try_swap_res = typename SwapProcType::try_swap_res;

public:
  Path
  solve() noexcept
  {
    auto no_swap_cycles = 0;
    constexpr auto no_swap_breakdown = 500;
    auto cycle_count = 100'000;
    this->reset();
    this->list.reset();
    do {
      swap_candidate best_swap{};
      best_swap.cost = cost_inf;

      for (index_t i = 0; i < this->matrix.size(); i++) {
        for (index_t j = i + 1; j < this->matrix.size(); j++) {
          auto new_cost = this->try_swap(i, j);

          if (new_cost.amor_cost < best_swap.cost)
            best_swap = { i, j, new_cost.cost };
        }
      }

      this->swap(best_swap.v1, best_swap.v2);
      this->list.add_tabu(best_swap.v1, best_swap.v2);

      ++no_swap_cycles;

      if (best_swap.cost < this->cost) {
        this->optimal_path = this->current_path;
        this->cost = pea::cost(this->matrix, this->optimal_path);
        no_swap_cycles = 0;
      }

      this->list.cycle();
      cycle_count--;

      if (no_swap_cycles == no_swap_breakdown) {
        no_swap_cycles = 0;
        this->current_path = Path::generate_random(this->matrix.size());
      }
    } while (cycle_count >= 0);

    return this->optimal_path;
  }

private:
  struct swap_candidate
  {
    index_t v1, v2;
    cost_t cost;
  };

  void
  swap(MSTMatrix::iterator a, MSTMatrix::iterator b) noexcept
  {
    this->swapper(a, b);
  }

  void
  swap(index_t v1_idx, index_t v2_idx) noexcept
  {
    this->swapper(this->current_path.it_at(v1_idx),
                  this->current_path.it_at(v2_idx));
  }

  try_swap_res
  try_swap(index_t v1_idx, index_t v2_idx) noexcept
  {
    return this->swapper.try_swap(this->current_path.it_at(v1_idx),
                                  this->current_path.it_at(v2_idx));
  }


private:
  MSTMatrix matrix;
  Path current_path;
  Path optimal_path;
  cost_t cost;
  tabu_list list;
  SwapProcType swapper;
};

} // namespace pea
