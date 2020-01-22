#include "aco.h"
#include "timeutils.hpp"

#include <cstdlib>
#include <ctime>
#include <fmt/format.h>

using namespace pea;

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  srand(time(0));

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  // tabu_solver<swap_op, init_strat_e::random> t(std::move(matrix));
  // genetic::solver<genetic::cross_strat::ox,
  // genetic::select_strat::ranking, 35>
  aco::solver<> t(std::move(matrix));
  auto p = t.solve();
  auto cost = pea::cost(t.get_matrix(), p);
  fmt::print("Cost: {}\n", cost);
}
