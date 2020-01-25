enum class AcoType
{ DAS, QAS };

template<AcoType aco_type_>
class Ant : public AntBase
{
public:
  static constexpr auto aco_type = aco_type_;

  Ant(const MSTMatrix &m, MSTMatrix &p)
  : AntBase(m, p) {} // Simply forward arguments to base class

  void move() noexcept;
};
