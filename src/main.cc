#include "mst.hpp"
#include "path.hpp"
#include "hk.hpp"
#include "bt.hpp"
#include "timeutils.hpp"

#include <fmt/format.h>

using namespace pea;

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  auto p = pea::hksolve(matrix);

  fmt::print("Cost:{}\n", p[0]);

}
