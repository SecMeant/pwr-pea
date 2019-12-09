#include "sa.hpp"

#include <fmt/format.h>
#include <cstdlib>
#include <ctime>

using namespace pea;

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  srand(time(0));

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  simann<SwapProcType::swap, TempProcType::standard> t(std::move(matrix));
  auto p = t.solve();
  auto cost = t.get_cost();
  fmt::print("Cost: {}\n", cost);
  p.display();
}
