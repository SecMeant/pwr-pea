#pragma once
#include <algorithm>

#include "utils.hpp"
#include "path.hpp"

#include "ts.hpp"

namespace pea {
  struct swap_op
  {
    using parent_t = tabu<swap_op>;

    void
    operator()(Path::iterator a, Path::iterator b)
    {
      this->swap(a,b);
    }

    void
    swap(Path::iterator a, Path::iterator b)
    {
      std::iter_swap(a, b);
    }

    struct try_swap_res
    {
      cost_t cost;
      cost_t amor_cost;
    };

    try_swap_res
    try_swap(Path::iterator a, Path::iterator b)
    {
      auto parent = container_of(this, parent_t, swapper);
      try_swap_res res;

      this->swap(a,b);
      res.cost = pea::cost(parent->matrix, parent->current_path);
      res.amor_cost = res.cost;
      this->swap(a,b);

      return res;
    }
  };

  struct insert_op
  {
    using parent_t = tabu<insert_op>;

    void
    operator()(Path::iterator a, Path::iterator b)
    {
      this->swap(a,b);
    }

    void
    swap(Path::iterator a, Path::iterator b)
    {
      if (a < b) {

        auto copy = std::move(*b);
        auto it = b;

        while(it != a) {
          *it = std::move(*(it-1));
          std::advance(it, -1);
        }

        *a = std::move(copy);

      } else {

        auto copy = std::move(*b);
        auto it = b;

        while(it != a) {
          *it = std::move(*(it+1));
          std::advance(it, 1);
        }

        *a = std::move(copy);

      }
    }

    struct try_swap_res
    {
      cost_t cost;
      cost_t amor_cost;
    };

    try_swap_res
    try_swap(Path::iterator a, Path::iterator b)
    {
      auto parent = container_of(this, parent_t, swapper);
      try_swap_res res;

      this->swap(a,b);
      res.cost = pea::cost(parent->matrix, parent->current_path);
      res.amor_cost = res.cost;
      this->swap(a,b);

      return res;
    }

  };

  struct invert_op
  {
    using parent_t = tabu<invert_op>;

    void
    operator()(Path::iterator a, Path::iterator b)
    {
      this->swap(a,b);
    }

    void
    swap(Path::iterator a, Path::iterator b)
    {
      std::reverse(a,std::next(b));
    }

    struct try_swap_res
    {
      cost_t cost;
      cost_t amor_cost;
    };

    try_swap_res
    try_swap(Path::iterator a, Path::iterator b)
    {
      auto parent = container_of(this, parent_t, swapper);
      try_swap_res res;

      this->swap(a,b);
      res.cost = pea::cost(parent->matrix, parent->current_path);
      res.amor_cost = res.cost;
      this->swap(a,b);

      return res;
    }
  };
} // namespace pea
