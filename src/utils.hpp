#pragma once
#include <algorithm>

template<typename It>
bool
next_permutation(It begin, It end)
{
  if (begin == end)
    return false;

  It i = begin;
  ++i;
  if (i == end)
    return false;

  i = end;
  --i;

  while (true) {
    It j = i;
    --i;

    if (*i < *j) {
      It k = end;

      while (!(*i < *--k))
        /* pass */;

      iter_swap(i, k);
      std::reverse(j, end);
      return true;
    }

    if (i == begin) {
      std::reverse(begin, end);
      return false;
    }
  }
}
