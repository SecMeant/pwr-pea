#pragma once
#include <limits>
#include <numeric>

#include <cstdlib>

#include "mst.hpp"
#include "typealias.hpp"
#include "utils.hpp"

namespace pea {

  using point_type = size_t;

  class Path : public std::vector<point_type>
  {
  public:
    Path(std::initializer_list<point_type> il) noexcept
      : std::vector<point_type>(il)
    {}

    template<typename... Args>
    Path(Args &&... args) noexcept
      : std::vector<point_type>(std::forward<Args>(args)...)
    {}

    static Path
    generate_simple(size_t node_count) noexcept
    {
      std::vector<point_type> v(node_count);
      std::iota(v.begin(), v.end(), 0);
      return v;
    }

    static Path
    generate_random(size_t node_count) noexcept
    {
      std::vector<point_type> v = generate_simple(node_count);
      for (auto i = 0u; i < node_count; ++i)
        std::swap(v[rand()%node_count], v[rand()%node_count]);
      return v;
    }

    void
    display() const noexcept
    {
      for (auto &p : *this)
        fmt::print("{} -> ", p);
    }

    bool
    verify() const noexcept
    {
      auto vbuf = std::make_unique<point_type[]>(this->size());
      std::fill_n(vbuf.get(), this->size(), 0);

      for (auto &e : *this)
        ++vbuf[e];

      for (auto i = 0ull; i < this->size(); ++i) {
        if (vbuf[i] != 1)
          return false;
      }

      return true;
    }

    iterator it_at(size_type idx)
    {
      return this->begin() + idx;
    }
  };

  constexpr cost_t cost_inf = std::numeric_limits<cost_t>::max();

  cost_t
  cost(const MSTMatrix &matrix, const Path &path) noexcept;

  class ScoredPath : public Path
  {
  public:
    ScoredPath() = default;

    ScoredPath(const ScoredPath &other)
    : Path(other)
    , cost(other.cost)
    , age(other.age)
    {
    }

    ScoredPath(ScoredPath &&other)
    : Path(std::move(other))
    , cost(other.cost)
    , age(other.age)
    {
    }

    ScoredPath& operator=(const ScoredPath &other)
    {
      static_cast<Path&>(*this) = static_cast<const Path&>(other);
      this->cost = other.cost;
      this->age = other.age;
      return *this;
    }

    ScoredPath& operator=(ScoredPath &&other)
    {
      static_cast<Path&>(*this) = static_cast<Path&&>(other);
      this->cost = other.cost;
      this->age = other.age;
      return *this;
    }

    ScoredPath(Path &&p)
    : Path(std::move(p))
    , cost(cost_inf)
    {
    }

    cost_t
    get_cost() const noexcept
    {
      return this->cost;
    }

    void
    update_cost(const MSTMatrix &matrix) noexcept
    {
      this->cost = pea::cost(matrix, *this);
    }

    bool operator==(const ScoredPath &other)
    {
      auto& this_  = static_cast<const Path&>(*this);
      auto& other_ = static_cast<const Path&>(other);
      return this_ == other_;
    }

    void
    reset_age()
    {
      this->age = 0;
    }

    void
    increment_age() noexcept
    {
      ++this->age;
    }

    age_t
    get_age() const noexcept FORCE_INLINE
    { return this->age; }

    friend cost_t
    cost(const MSTMatrix &matrix, const ScoredPath &path) noexcept;

  private:
    ::cost_t cost = 313371337;
    age_t age = 0;
  };


} // namespace pea
