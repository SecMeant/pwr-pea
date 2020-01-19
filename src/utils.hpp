#pragma once
#include <algorithm>

#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define FORCE_INLINE __attribute__((always_inline))

#ifndef NDEBUG
#define BUGON(x) {if(x) {assert(false);}}
#else
#define BUGON(x)
#endif

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

template<class P, class M>
size_t my_offsetof(const M P::*member)
{
    return (size_t) &( reinterpret_cast<P*>(0)->*member);
}

template<class P, class M>
P* container_of_(M* ptr, const M P::*member)
{
    return (P*)( (char*)ptr - my_offsetof(member));
}

#define container_of(ptr, type, member) \
     container_of_(ptr, &type::member)

