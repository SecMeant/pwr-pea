#include "mst.hpp"

#include <fmt/format.h>
#include <random>
#include <string>
#include <fstream>

#include "debug_print.h"

namespace pea {

  MSTMatrix::MSTMatrix(int32_t size) noexcept
  {
    this->tree =
      new std::remove_pointer<decltype(this->tree)>::type[size * size];
    this->size = size;

    std::fill(this->tree, this->tree + (size * size), 0);
  }

  MSTMatrix::MSTMatrix(MSTMatrix &&mm) noexcept
    : tree(mm.tree)
    , size(mm.size)
  {
    mm.tree = nullptr;
    mm.size = 0;
  }

  MSTMatrix::~MSTMatrix() { delete[] this->tree; }

  void
  MSTMatrix::add_single(Edge edge) noexcept
  {
    this->set(edge.v1, edge.v2, edge.weight);
  }

  void
  MSTMatrix::add(Edge edge) noexcept
  {
    this->set(edge.v1, edge.v2, edge.weight);
    this->set(edge.v2, edge.v1, edge.weight);
  }

  int32_t
  MSTMatrix::get(size_t x, size_t y)
  {
    assert(x < this->size);
    assert(y < this->size);

    return this->tree[x + y * this->size];
  }

  void
  MSTMatrix::set(size_t x, size_t y, int64_t val)
  {
    assert(x < this->size);
    assert(y < this->size);

    this->tree[x + y * this->size] = val;
  }

  void
  MSTMatrix::resize(size_t newsize) noexcept
  {
    delete[] this->tree;
    this->tree = new int64_t[newsize * newsize]();
    this->size = newsize;
  }

  void
  MSTMatrix::display() noexcept
  {
    fmt::print("{:<5}", " ");
    for (size_t x = 0; x < this->size; ++x) {
      fmt::print("{:<3} ", x);
    }
    putchar('\n');

    for (size_t x = 0; x < this->size + 1; ++x) {
      fmt::print("----");
    }
    putchar('\n');

    for (size_t y = 0; y < this->size; ++y) {
      fmt::print("{:<2} | ", y);
      for (size_t x = 0; x < this->size; ++x) {
        fmt::print("{:<3} ", this->get(x, y));
      }
      putchar('\n');
    }
  }

#define CHECK_STREAM(stream) \
  if (stream.fail()) { \
    debug_printerr("Failed\n"); \
    return {}; \
  }

  MSTMatrix
  MSTMatrix::buildFromFile(const char *filename)
  {
    std::ifstream is(filename);

    // Seems like each file starts with data name.
    std::string data_name;
    is >> data_name;
    CHECK_STREAM(is);

    size_t node_count;
    is >> node_count;
    CHECK_STREAM(is);

    MSTMatrix matrix(node_count);

    for(size_t y = 0; y < node_count; ++y) {
      for(size_t x = 0; x < node_count; ++x) {

        int64_t weight;
        is >> weight;
        CHECK_STREAM(is);

        matrix.set(x, y, weight);

      }
    }

    return matrix;
  }

#undef CHECK_STREAM

} // namespace pea
