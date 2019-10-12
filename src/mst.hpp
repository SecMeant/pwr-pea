#pragma once
#include <cstdint>
#include <fmt/ostream.h>

#include <list>

namespace pea {
  struct Edge
  {
    typedef int32_t base_type;

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

    friend std::ostream &
    operator<<(std::ostream &os, const Edge &e)
    {
      return os << e.v1 << ' ' << e.v2 << ' ' << e.weight;
    }

    friend std::ifstream &
    operator>>(std::ifstream &is, Edge &e)
    {
      is >> e.v1 >> e.v2 >> e.weight;
      return is;
    }
  };

  class MSTMatrix
  {
  private:
    int32_t *tree;
    int32_t weight;
    int32_t size;

  public:
    MSTMatrix(int32_t size) noexcept;
    MSTMatrix(MSTMatrix &&mm) noexcept;
    ~MSTMatrix();

    void
    add_single(Edge edge) noexcept;

    void
    add(Edge edge) noexcept;

    int32_t
    get(int32_t x, int32_t y);

    void
    set(int32_t x, int32_t y, int32_t val);

    // Clears and resizes matrix
    void
    resize(int32_t newsize) noexcept;

    void
    display() noexcept;

    static MSTMatrix
    buildFromFile(const char *filename);

    static MSTMatrix
  };

}; // namespace pea
