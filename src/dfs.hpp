#pragma once
#include "mst.hpp"
#include "path.hpp"

#include <algorithm>
#include <limits>

namespace pea {

  class tspdfs
  {
  private:
    const MSTMatrix &matrix;
    Path rpath; // result path
    cost_t rcost;
    Path lpath; // local path
    cost_t lcost;
    bool *visited;

  public:
    tspdfs(const MSTMatrix &matrix) noexcept
      : matrix(matrix)
      , rpath()
      , rcost(std::numeric_limits<cost_t>::max())
      , lpath()
      , lcost(0)
      , visited(new bool[matrix.size()])
    {
      std::fill(visited, visited + matrix.size(), false);
    }

    void
    reset() noexcept
    {
      rpath.clear();
      rcost = std::numeric_limits<cost_t>::max();
      lpath.clear();
      lcost = 0;
      std::fill(visited, visited + matrix.size(), false);
    }

    ~tspdfs() noexcept { delete[] visited; }

    const Path &
    result() const noexcept
    {
      return this->rpath;
    }

    // puts result in cpath
    void
    solve() noexcept
    {
      TSP(0);
    }

  private:
    void
    TSP(size_t v) noexcept;
  };

  Path
  dfs(const MSTMatrix &matrix)
  {
    tspdfs solver(matrix);
    solver.solve();

    return solver.result();
  }

} // namespace pea
