#pragma once

#include "mst.hpp"
#include "path.hpp"
#include "utils.hpp"

#include <array>
#include <iterator>
#include <algorithm>
#include <utility>
#include <cstdlib>

namespace pea::genetic {
  enum class cross_strat
  {
    ox,
    pmx,
    px,
    rndswp,
  };

  enum class select_strat
  {
    roulette,
    competition,
    ranking
  };

  template<cross_strat xs, select_strat ss, size_t mating_pool_size_>
  class solver
  {
  public:
    static constexpr auto xstrat = xs;
    static constexpr auto sstrat = ss;
    static constexpr auto mating_pool_size = mating_pool_size_;
    static constexpr auto mutation_probability = 0.1;
    static constexpr auto max_mut_count = mating_pool_size;

    using mating_pool_t = std::array<ScoredPath, mating_pool_size>;

    template<typename MatrixType>
    inline solver(MatrixType &&m)
      : matrix(std::forward<MatrixType>(m))
    {}

    solver(const solver &t) = default;
    solver(solver &&t) = default;

    solver &
    operator=(const solver &t) = default;

    solver &
    operator=(solver &&t) = default;

    ~solver() = default;

  private:
    inline static void
    pmx_patch_(ScoredPath &p, const ScoredPath &p1, const ScoredPath &p2, size_t i, size_t xbegin, size_t xend) FORCE_INLINE
    {
      auto oldnode = p[i];
      point_type newnode;

      const point_type *newnode_p = &p[i];
      while(1)
      {
        newnode = *newnode_p;
        newnode_p = std::find(&p2[xbegin], &p2[xend], oldnode);

        if (newnode_p == &p2[xend])
          break;

        oldnode = p1[newnode_p - &p2[0]];
      }

      p[i] = oldnode;
    }

    ScoredPath
    cross_pmx(const ScoredPath &p1, const ScoredPath &p2)
    {
      const auto path_size = std::size(p1);
      ScoredPath p(p1);

      BUGON(p1.size() != p2.size());

      // If range starts at the end, then its bad range, so avoid
      // that with shrinking span of path_size.
      auto xbegin = std::rand() % (path_size - 1);

      // xend should point to past last element
      auto xend   = std::rand() % (path_size + 1);

      if (UNLIKELY(xbegin == xend)) ++xend;
      else if (xbegin > xend) std::swap(xbegin, xend);

      xbegin = 3;
      xend = 7;
      std::fill(&p[xbegin], &p[xend], 1337);

      for (auto i = 0ull; i < xbegin; ++i) {
        pmx_patch_(p, p1, p2, i, xbegin, xend);
      }

      for (auto i = xend; i < path_size; ++i) {
        pmx_patch_(p, p1, p2, i, xbegin, xend);
      }

      std::copy(&p2[xbegin], &p2[xend], &p[xbegin]);

      return p;
    }

    ScoredPath
    cross_rndswp(const ScoredPath &p1, const ScoredPath &p2)
    {
      ScoredPath p = p1;

      auto mut_count = rand() % p1.size();

      for (auto i = 0ull; i < mut_count; ++i) {
        auto pos = rand() % p1.size();
        auto oldnode = p[pos];
        auto newnode = p2[pos];

        auto patch = std::find(std::begin(p), std::end(p), newnode);
        BUGON(patch == std::end(p));
        p[pos] = newnode;
        *patch = oldnode;
      }

      return p;

    }

    ScoredPath
    cross_ox(const ScoredPath &p1, const ScoredPath &p2)
    {
      const auto path_size = std::size(p1);
      ScoredPath p(path_size);

      BUGON(p1.size() != p2.size());

      // If range starts at the end, then its bad range, so avoid
      // that with shrinking span of path_size.
      auto xbegin = std::rand() % (path_size - 1);

      // xend should point to past last element
      auto xend   = std::rand() % (path_size + 1);

      if (UNLIKELY(xbegin == xend)) ++xend;
      if (xbegin > xend) std::swap(xbegin, xend);

      // Fill with invalid nodes
      std::fill_n(std::begin(p), p.size(), 999999);

      // Copy random part from p1 to child
      std::copy(&p1[xbegin], &p1[xend], &p[xbegin]);

      // Copy each node from p2 that is not in child
      auto j = xend;
      for (auto &e : p2) {
        if (std::find(std::begin(p), std::end(p), e) == std::end(p)) {
          if (j == path_size)
            j = 0;

          p[j] = e;
          ++j;
        }
      }

      return p;
    }

    ScoredPath
    cross(const ScoredPath &p1, const ScoredPath &p2) FORCE_INLINE
    {
      if constexpr (xstrat == cross_strat::pmx) {
        return cross_pmx(p1, p2);
      } else if constexpr (xstrat == cross_strat::rndswp) {
        return cross_rndswp(p1, p2);
      } else if constexpr (xstrat == cross_strat::ox) {
        return cross_ox(p1, p2);
      }

      *(int*)nullptr = 1;
    }

    void
    mutate(ScoredPath &p)
    {
      constexpr auto mutprob = static_cast<int>(mutation_probability * 100.0);
      const auto prob = rand() % 100;

      if (prob >= mutprob)
        return;

      auto mut_count = rand() % max_mut_count;

      while(mut_count > 0) {
        const auto mutpos1 = rand() % p.size();
        const auto mutpos2 = rand() % p.size();

        std::swap(p[mutpos1], p[mutpos2]);

        --mut_count;
      }
    }

    void
    sorted_insert(ScoredPath &&p)
    {
      auto it = std::begin(this->mating_pool) + this->mating_pool.size() / 3;
      auto mpend = std::end(this->mating_pool);
      p.update_cost(this->matrix);
      auto pcost = p.get_cost();

      while(it < mpend && pcost < it->get_cost()) {
        ++it;
      }

      auto ip = std::prev(it);

      auto fi = std::find(std::begin(this->mating_pool),
                    std::end(this->mating_pool),
                    p);

      if (fi != std::end(this->mating_pool))
        return;

      while (ip >= std::begin(this->mating_pool)) {
        std::swap(*ip, p);
        --ip;
      }

      auto &local_best = this->mating_pool.back();
      if (local_best.get_cost() < this->best.get_cost()) {
        this->best = local_best; 
      }

    }

    template<typename ContainerType, typename OperationType>
    void
    for_each_combination(ContainerType &&container, OperationType &&op)
    {
      const auto elem_count = std::size(container);

      for (auto i = 0ull; i < elem_count; ++i) {
        for (auto j = 0ull; j < elem_count; ++j) {
          op(container[i], container[j]);
        }
      }

    }

  public:
    void
    reset()
    {
      if (this->matrix.size() <= 2)
        return;

      best = {};

      for (auto &p : this->mating_pool) {
        p = ScoredPath::generate_random(this->matrix.size());
        p.update_cost(this->matrix);
      }

      std::sort(std::begin(this->mating_pool),
                std::end(this->mating_pool),
                [](const ScoredPath& p1, const ScoredPath& p2) {
                  return p1.get_cost() > p2.get_cost();
                });
    }

    void
    stat() const noexcept
    {
      for (auto &e : this->mating_pool) {
        e.display();
        fmt::print("Cost {}\n", cost(this->matrix, e));
      }
    }

    ScoredPath
    solve_rank() noexcept
    {
      auto generation_count = 35 * this->matrix.size();
      mating_pool_t mp_copy;
      for (auto i = 0ul; i < generation_count; ++i) {

        mp_copy = this->mating_pool;

          for_each_combination(mp_copy,
            [this](ScoredPath &p1, ScoredPath &p2) {
              auto np = this->cross(p1, p2);
              this->mutate(np);
              BUGON(!np.verify());
              this->sorted_insert(std::move(np));
            }
          );

      }
    }

    ScoredPath
    solve() noexcept
    {
      this->reset();

      auto generation_count = 5 * this->matrix.size(); // 5
      mating_pool_t mp_copy;
      for (auto i = 0ul; i < generation_count; ++i) {

        mp_copy = this->mating_pool;

          for_each_combination(mp_copy,
            [this](ScoredPath &p1, ScoredPath &p2) {
              auto np = this->cross(p1, p2);
              this->mutate(np);
              BUGON(!np.verify());
              this->sorted_insert(std::move(np));
            }
          );
      }

      return this->best;
    }

    ScoredPath
    test()
    {
      ScoredPath p1 {{0,1,2,3,4,5,6,7}};
      ScoredPath p2 {{0,2,3,6,7,4,1,5}};

      auto p = cross(p1, p2);
      p.display();
      fmt::print("\n");
      BUGON(!p.verify());
      return p;
    }

    const auto&
    get_matrix()
    { return this->matrix; }

  private:
    MSTMatrix matrix;
    mating_pool_t mating_pool;
    ScoredPath best;
  };
} // namespace pea::genetic
