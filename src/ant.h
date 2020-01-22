#pragma once

#include "mst.hpp"
#include "path.hpp"
#include "utils.hpp"

#include <memory>

namespace pea {

  class AntBase
  {
  public:
    // Path value coeffiction, used for calculating path value
    static constexpr size_t pvcoeff = 8;

    // Amount of pheromone placed on the path
    static constexpr int64_t phro_place_amnt = 2048;

    AntBase(const MSTMatrix &m, MSTMatrix &phro)
      : matrix(m)
      , phro(phro)
      , tabu(std::make_unique<bool[]>(m.size()))
    {}

    void
    reset(point_type new_start_node) noexcept;

    point_type
    decide() noexcept; // Decide which way to go

    cost_t
    value(point_type from, point_type to) noexcept; // value road

    UNSAFE auto &&
    steal_path_() noexcept
    {
      return std::move(this->path);
    }

    auto
    get_cost() const noexcept
    {
      return this->cost;
    }

    UNSAFE point_type find_nth_unvisited_(size_t nth) const noexcept;

  private:
    inline size_t
    count_free_space() const noexcept FORCE_INLINE
    {
      return this->matrix.size() - this->path.size();
    }

  protected:
    Path path;
    cost_t cost;
    const MSTMatrix &matrix;
    MSTMatrix &phro;
    std::unique_ptr<bool[]> tabu;
  };

  enum class AcoType
  {
    DAS,
    QAS
  };

  template<AcoType aco_type_>
  class Ant : public AntBase
  {
  public:
    static constexpr auto aco_type = aco_type_;

    Ant(const MSTMatrix &m, MSTMatrix &p)
      : AntBase(m, p)
    {}

    void
    move() noexcept;
  };

  template<>
  inline void
  Ant<AcoType::DAS>::move() noexcept
  {
    point_type current_node = this->path.back();
    point_type next_node = this->decide();

    BUGON(current_node == next_node);
    BUGON(this->tabu[next_node]);

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
    point_type next_node = this->decide();

    BUGON(current_node == next_node);
    BUGON(this->tabu[next_node]);

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
} // namespace pea
