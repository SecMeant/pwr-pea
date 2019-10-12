#pragma once
#include <cstdint>

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
  };

  class MSTMatrix
  {
  private:
    int64_t *tree;
    size_t size;

  public:
    MSTMatrix() noexcept : MSTMatrix(0) {}
    MSTMatrix(int32_t size) noexcept;
    MSTMatrix(MSTMatrix &&mm) noexcept;
    ~MSTMatrix();

    void
    add_single(Edge edge) noexcept;

    void
    add(Edge edge) noexcept;

    int32_t
    get(size_t x, size_t y);

    void
    set(size_t x, size_t y, int64_t val);

    // Clears and resizes matrix
    void
    resize(size_t newsize) noexcept;

    void
    display() noexcept;

    // Returns matrix with zero size on failure
    static MSTMatrix
    buildFromFile(const char *filename);

  };

}; // namespace pea
