class AntBase
{
public:
  void
  reset(point_type new_start_node) noexcept;

  point_type
  decide() noexcept; // Decide which way to go

  cost_t
  value(point_type from, point_type to) noexcept; // value road
protected:
  Path path;
  cost_t cost;
  const MSTMatrix &matrix;
  MSTMatrix &phro;
  std::unique_ptr<bool[]> tabu;
};
