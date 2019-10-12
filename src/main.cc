#include "mst.hpp"

using namespace pea;

int main()
{
  auto matrix = MSTMatrix::buildFromFile("data.txt");
  matrix.display();
}
