#include "mst.hpp"
#include <fmt/format.h>

using namespace pea;

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  auto matrix = MSTMatrix::buildFromFile(argv[1]);
  matrix.display();

  fmt::print("\n\n");
  fmt::print("Matrix cost: {}\n",
             cost(matrix, Path::generate_simple(matrix.size())));
}
