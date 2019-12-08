#include "ts.hpp"
#include "ts_util.hpp"

#include <fmt/format.h>

using namespace pea;

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  tabu<pea::invert_op> t(std::move(matrix));
  auto p = t.solve();
  auto cost = t.get_cost();
  fmt::print("Cost: {}\n", cost);
  p.display();
}
