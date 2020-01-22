#pragma once
#include <algorithm>

#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define FORCE_INLINE __attribute__((always_inline))
// Used to mark functions that are generaly unsafe, and caller should
// make sure that he knows what he is doing.
#define UNSAFE

#ifndef NDEBUG
#define BUGON(x) {if(x) {assert(false);}}
#else
#define BUGON(x)
#endif

#define CANT_TOUCH_THIS __builtin_trap();

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

inline size_t
rand_range(size_t lbound, size_t ubound)
{
  if (UNLIKELY(ubound < lbound))
    std::swap(ubound, lbound);

  size_t range = ubound - lbound;
  return rand() % range + lbound;
}

inline size_t
rand_range(size_t ubound)
{
  return rand() % ubound;
}
