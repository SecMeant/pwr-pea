#include "bt.hpp"
#include "dfs.hpp"
#include "hk.hpp"
#include "mst.hpp"
#include "path.hpp"
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
}
