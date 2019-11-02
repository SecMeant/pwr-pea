#include "timeutils.hpp"
#include "mst.hpp"
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

void bt(MSTMatrix &matrix)
{
  cost_t btcost;
  auto btres = TSPBruteForce(matrix, &btcost);
  fmt::print("\n\nBruteforce\n");
  fmt::print("Matrix cost: {}\n", btcost);
}

void simple(MSTMatrix &matrix)
{
  fmt::print("\n\nSimple\n");
  fmt::print("Matrix cost: {}\n",
             cost(matrix, Path::generate_simple(matrix.size())));
}

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  measure_and_log("bt", "measure.log", bt, matrix);
}
