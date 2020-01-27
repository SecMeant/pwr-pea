#include "aco.h"
#include "genetic.hpp"
#include "timeutils.hpp"

#include <map>
#include <cstdlib>
#include <ctime>
#include <fmt/format.h>
#include <string>

using namespace pea;

constexpr auto filename_template = "/home/holz/Downloads/ATSP/ATSP/data{}.txt";
constexpr auto filenamelog_template = "log.txt";

std::string testfile(int problem_size)
{
  return fmt::format(filename_template, problem_size);
}

std::string testfilelog(int problem_size)
{
  return fmt::format(filenamelog_template, problem_size);
}

std::map<int, cost_t> best_scores {
  {17  ,   39},
  {34  , 1286},
  {36  , 1473},
  {39  , 1530},
  {43  , 5620},
  {45  , 1613},
  {48  ,14422},
  {53  , 6905},
  {56  , 1608},
  {65  , 1839},
  {70  ,38673},
  {71  , 1950},
  {100 ,36230},
  {171 , 2755},
  {323 , 1326},
  {358 , 1163},
  {403 , 2465},
  {443 , 2720},
};

int
main(int argc, char **argv)
{
  if (argc != 2)
    return 1;

  srand(time(0));

  auto psize = 0;
  psize = atoi(argv[1]);

  if (psize == 0)
    return -1;

  std::string f_name = testfile(psize);
  auto matrix = MSTMatrix::buildFromFile(f_name.c_str());

  FILE *f_out = fopen(testfilelog(psize).c_str(), "a");

   //tabu_solver<swap_op, init_strat_e::random> t(std::move(matrix));
   //genetic::solver<genetic::cross_strat::ox,
   //genetic::select_strat::competition, 25> t(std::move(matrix)); // 25
  aco::solver<> t(std::move(matrix));
  auto p = t.solve();
  auto cost = pea::cost(t.get_matrix(), p);
  auto bestcost = best_scores[psize];
  fmt::print(" {} {} {}%\n", bestcost, cost, static_cast<float>((cost - bestcost)) / static_cast<float>(bestcost) * 100.0f);

  fclose(f_out);
}
