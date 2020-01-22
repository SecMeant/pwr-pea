#pragma once
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <vector>

#include <fmt/format.h>

namespace pea {
  struct Edge
  {
    // TODO make v1 and v2 be unsigned type
    typedef int64_t base_type;

    base_type v1;
    base_type v2;
    base_type weight;

    inline Edge()
      : v1{ 0 }
      , v2{ 0 }
      , weight{ 0 }
    {}
    inline Edge(base_type v1, base_type v2, base_type weight)
      : v1{ v1 }
      , v2{ v2 }
      , weight{ weight }
    {}

    inline int32_t
    getKey()
    {
      return weight;
    }

    inline bool
    operator==(Edge e)
    {
      return ((this->v1 == e.v1 && this->v2 == e.v2) ||
              (this->v1 == e.v2 && this->v2 == e.v1)) &&
             this->weight == e.weight;
    }

    inline bool
    operator!=(Edge e)
    {
      return !(*this == e);
    }
  };

  class MSTMatrix
  {
  public:
    using value_type = int64_t;
    using iterator = value_type *;
    using const_iterator = const value_type *;

    using index_type = size_t;

    MSTMatrix() noexcept
      : MSTMatrix(0)
    {}

    MSTMatrix(int64_t size) noexcept;
    MSTMatrix(MSTMatrix &&mm) noexcept;
    ~MSTMatrix();

    void
    add_single(Edge edge) noexcept;

    void
    add(Edge edge) noexcept;

    value_type
    get(index_type x, index_type y) const noexcept;

    value_type
    safe_get(index_type x, index_type y) const noexcept;

    void
    set(index_type x, index_type y, int64_t val);

    // Clears and resizes matrix
    void
    resize(index_type newsize) noexcept;

    void
    display() const noexcept;

    // Returns matrix with zero size on failure
    static MSTMatrix
    buildFromFile(const char *filename);

    constexpr auto
    size() const noexcept
    {
      return this->m_size;
    }

    constexpr auto
    data() const noexcept
    {
      return this->m_data;
    }

    auto
    begin() const noexcept
    {
      return this->m_data;
    }

    auto
    end() const noexcept
    {
      return this->m_data + this->m_size;
    }

    Edge
    nearest_neighbour(value_type node) const noexcept
    {
      auto pred = []([[maybe_unused]] index_type _){return true;};
      return this->nearest_neighbour_if(node, pred);
    }

    template<typename PredicateType>
    Edge
    nearest_neighbour_if(value_type node, PredicateType &&predicate) const noexcept
    {
      // TODO change nearest to be type of index_type
      value_type nearest = -1;
      value_type nearest_cost = std::numeric_limits<value_type>::max();

      for (index_type i = 0; i < this->m_size; ++i) {

        auto cost = this->get(node, i);

        if (cost < nearest_cost && predicate(i)) {
          nearest = i;
          nearest_cost = cost;
        }

      }

      return  {node, nearest, nearest_cost};
    }

  private:
    value_type *m_data;
    index_type m_size;

  };

}; // namespace pea
