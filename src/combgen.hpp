#pragma once

#include <bitset>
#include <iostream>
#include <vector>

using uint = unsigned int;

template<size_t size>
using subset_t = std::bitset<size>;

// THIS IMPLEMENTATION IS USEABLE ONLY BY HELD-KARP ALGO !
// TODO move to hk impl file
template<size_t bitset_size, typename subset_type = subset_t<bitset_size>>
static void
comb(subset_type set,
     uint at,
     uint r,
     uint n,
     std::vector<subset_type> &subsets)
{
  if (r == 0) {

    // Must include starting node
    if (set[0])
      subsets.push_back(set);

    return;
  }

  for (uint i = at; i < n; i++) {
    set |= (1 << i);
    comb<bitset_size>(set, i + 1, r - 1, n, subsets);
    set &= ~(1 << i);
  }
}

// THIS IMPLEMENTATION IS USEABLE ONLY BY HELD-KARP ALGO !
// TODO move to hk impl file
template<size_t bitset_size, typename subset_type = subset_t<bitset_size>>
std::vector<subset_type>
comb(uint r, uint n)
{
  std::vector<subset_type> subsets;
  comb<bitset_size>(std::bitset<bitset_size>(0), 0, r, n, subsets);
  return subsets;
}
