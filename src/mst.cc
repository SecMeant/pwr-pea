#include "mst.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <iterator>
#include <exception>
#include <random>
#include <fmt/format.h>

pea::MSTMatrix::MSTMatrix(int32_t size) noexcept
{
  this->tree = new std::remove_pointer<decltype(this->tree)>::type[size*size];
  this->weight = 0;
  this->size = size;

  std::fill(this->tree, this->tree+(size*size), 0);
}

pea::MSTMatrix::MSTMatrix(MSTMatrix &&mm) noexcept
:tree(mm.tree), weight(mm.weight), size(mm.size)
{
  mm.tree = nullptr;
  mm.weight = 0;
  mm.size = 0;
}

pea::MSTMatrix::~MSTMatrix()
{
  delete [] this->tree;
}

void pea::MSTMatrix::add_single(pea::Edge edge) noexcept
{
  assert(edge.v1 < this->size);
  assert(edge.v2 < this->size);

  this->set(edge.v1, edge.v2, edge.weight);
  this->weight += edge.weight;
}

void pea::MSTMatrix::add(pea::Edge edge) noexcept
{
  assert(edge.v1 < this->size);
  assert(edge.v2 < this->size);

  this->set(edge.v1, edge.v2, edge.weight);
  this->set(edge.v2, edge.v1, edge.weight);

  this->weight += edge.weight;
}

int32_t pea::MSTMatrix::get(int32_t x, int32_t y)
{
  assert(x < this->size);
  assert(x >= 0);
  assert(y < this->size);
  assert(y >= 0);

  return this->tree[x + y * this->size];
}

void pea::MSTMatrix::set(int32_t x, int32_t y, int32_t val)
{
  assert(x < this->size);
  assert(x >= 0);
  assert(y < this->size);
  assert(y >= 0);

  this->tree[x + y * this->size] = val;
}

void pea::MSTMatrix::resize(int32_t newsize) noexcept
{
  delete [] this->tree;
  this->tree = new int32_t[newsize*newsize]();
  this->size = newsize;
  this->weight = 0;
}

void pea::MSTMatrix::display() noexcept
{
  fmt::print("Weight: {}\n", this->weight);

  fmt::print("{:<5}"," ");
  for(auto x = 0; x < this->size; ++x)
  {
    fmt::print("{:<3} ", x);
  }
  putchar('\n');

  for(auto x = 0; x < this->size+1; ++x)
  {
    fmt::print("----");
  }
  putchar('\n');

  for(auto y = 0; y < this->size; ++y)
  {
    fmt::print("{:<2} | ", y);
    for(auto x = 0; x < this->size; ++x)
    {
      fmt::print("{:<3} ", this->get(x,y));
    }
    putchar('\n');
  }
}

pea::MSTMatrix
pea::MSTMatrix::buildFromFile(const char *filename)
{
  std::ifstream file(filename);

  if(!file.is_open())
    throw std::runtime_error(fmt::format("Cant open file {}", filename));

  int32_t node_cnt;
  std::string line;

  // Ignore filename
  std::getline(file, line);

  file >> node_cnt;

  MSTMatrix matrix(node_cnt);

  return matrix;
}

