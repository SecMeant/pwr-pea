#include "timeutils.hpp"
#include "mst.hpp"
#include "path.hpp"
#include "utils.hpp"
#include <limits>
#include <fmt/format.h>

using namespace pea;

Path TSPBruteForce(const MSTMatrix& matrix, cost_t *costp = nullptr)
{
  Path p = Path::generate_simple(matrix.size());
  Path res;
  cost_t rescost = std::numeric_limits<decltype(rescost)>::max();
  
  do
  {
    cost_t tmp_cost = cost(matrix, p);

    if (tmp_cost < rescost) {
      rescost = tmp_cost;
      res = p;
    }

  }while(::next_permutation(std::begin(p), std::end(p)));

  if (costp)
    *costp = rescost;

  return res;
}

class tspdfs
{
private:
  const MSTMatrix& matrix;
  Path rpath; // result path
  cost_t rcost;
  Path lpath; // local path
  cost_t lcost;
  bool *visited;

public:
  tspdfs(const MSTMatrix& matrix)
  : matrix(matrix)
  , rpath()
  , rcost(std::numeric_limits<cost_t>::max())
  , lpath()
  , lcost(0)
  , visited(new bool[matrix.size()])
  {
    std::fill(visited, visited + matrix.size(), false);
  }

  void reset()
  {
    rpath.clear();
    rcost = std::numeric_limits<cost_t>::max();
    lpath.clear();
    lcost = 0;
    std::fill(visited, visited + matrix.size(), false);
  }

  ~tspdfs()
  { delete [] visited; }

  const Path& result() const
  { return this->rpath; }

  // puts result in cpath
  void solve()
  {
    TSP(0);
  }

private:
  void TSP(size_t v)
  {
    size_t u;

    lpath.push_back(v);

    if (lpath.size() < matrix.size()) {

      visited[v] = true;

      for (u = 0; u < matrix.size(); u++) {

        if (!visited[u]){
          lcost += matrix.get(v,u);
          TSP(u);
          lcost -= matrix.get(v,u);
        }

      }

      visited[v] = false;

    } else { // Path is complete, close it
      lcost += matrix.get(v, 0);

      // If local result is better than currently best
      // Copy local to best
      if (lcost < rcost) {
        rcost = lcost;
        rpath = lpath;
      }

      lcost -= matrix.get(v, 0);
    }

    lpath.pop_back();
  }
};

static void bt(MSTMatrix &matrix)
{
  cost_t btcost;
  auto btres = TSPBruteForce(matrix, &btcost);
  fmt::print("\n\nBruteforce\n");
  fmt::print("Matrix cost: {}\n", btcost);
}

static void simple(MSTMatrix &matrix)
{
  fmt::print("\n\nSimple\n");
  fmt::print("Matrix cost: {}\n",
             cost(matrix, Path::generate_simple(matrix.size())));
}

static void dfs(const MSTMatrix& matrix)
{
  tspdfs solver(matrix);
  solver.solve();

  fmt::print("\n\nDFS\n");
  fmt::print("Matrix cost: {}\n", cost(matrix, solver.result()));
}

const char *measure_filename = "measure.log";

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  bt(matrix);
  dfs(matrix);
}
