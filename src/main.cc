#include "ts.hpp"
#include "ts_util.hpp"

#include "sa.hpp"

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
  simann<SwapProcType::swap, TempProcType::log, init_strat_e::nearest_neighbour>
    t(std::move(matrix));
  auto p = t.solve();
  auto cost = t.get_cost();
  fmt::print("Cost: {}\n", cost);
  p.display();
}
