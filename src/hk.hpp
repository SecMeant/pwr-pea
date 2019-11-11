#include <bitset>
#include <vector>
#include <cstdint>

#include "path.hpp"
#include "typealias.hpp"

namespace pea {
  template<size_t size>
  using subset_t = std::bitset<size>;

  template<size_t bitset_size,
           typename subset_type = subset_t<bitset_size>>
  static void
  comb(subset_type set,
       uint at,
       uint r,
       uint n,
       std::vector<subset_type> &subsets)
  {
    if (r == 0) {
      subsets.push_back(set);
      return;
    }

    for (uint i = at; i < n; i++) {
      set |= (1 << i);
      comb<bitset_size>(set, i + 1, r - 1, n, subsets);
      set &= ~(1 << i);
    }
  }

  template<size_t bitset_size,
           typename subset_type = subset_t<bitset_size>>
  std::vector<subset_type>
  comb(uint r, uint n)
  {
    std::vector<subset_type> subsets;
    comb<bitset_size>(std::bitset<bitset_size>(0), 0, r, n, subsets);
    return subsets;
  }

  class MemoTable
  {
    using value_t = int64_t;

    value_t *m_data;
    size_t width;

  public:
    inline explicit MemoTable(size_t width, size_t height) noexcept
    :m_data(new value_t[width*height]), width(width) {}

    MemoTable(const MemoTable& other) noexcept = delete;
    MemoTable(MemoTable&& other) noexcept = delete;
    void operator=(const MemoTable &other) noexcept = delete;
    void operator=(MemoTable &&other) noexcept = delete;

    ~MemoTable() noexcept
    { delete [] this->m_data; }

    void set(size_t x, size_t y, value_t value) noexcept
    { this->m_data[x + y*this->width] = value; }

    value_t get(size_t x, size_t y) const noexcept
    { return this->m_data[x + y*this->width]; }
  };

  Path hksolve(const MSTMatrix &matrix);

} // namespace pea
