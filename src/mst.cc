#include "mst.hpp"

#include <fmt/format.h>
#include <fstream>
#include <random>
#include <string>
#include <limits>

#include "debug_print.h"

namespace pea {

  MSTMatrix::MSTMatrix(int64_t size) noexcept
  {
    this->m_data =
      new std::remove_pointer<decltype(this->m_data)>::type[size * size];
    this->m_size = size;

    std::fill(this->m_data, this->m_data + (size * size), 0);
  }

  MSTMatrix::MSTMatrix(MSTMatrix &&mm) noexcept
    : m_data(mm.m_data)
    , m_size(mm.m_size)
  {
    mm.m_data = nullptr;
    mm.m_size = 0;
  }

  MSTMatrix::~MSTMatrix() { delete[] this->m_data; }

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

  MSTMatrix::value_type
  MSTMatrix::get(index_type x, index_type y) const noexcept
  {
    assert(x < this->m_size);
    assert(y < this->m_size);

    return this->m_data[x + y * this->m_size];
  }

  void
  MSTMatrix::set(index_type x, index_type y, int64_t val)
  {
    assert(x < this->m_size);
    assert(y < this->m_size);

    this->m_data[x + y * this->m_size] = val;
  }

  void
  MSTMatrix::resize(index_type newsize) noexcept
  {
    delete[] this->m_data;
    this->m_data = new int64_t[newsize * newsize]();
    this->m_size = newsize;
  }

  void
  MSTMatrix::display() const noexcept
  {
    fmt::print("{:<5}", " ");
    for (index_type x = 0; x < this->m_size; ++x) {
      fmt::print("{:<3} ", x);
    }
    putchar('\n');

    for (index_type x = 0; x < this->m_size + 1; ++x) {
      fmt::print("----");
    }
    putchar('\n');

    for (index_type y = 0; y < this->m_size; ++y) {
      fmt::print("{:<2} | ", y);
      for (index_type x = 0; x < this->m_size; ++x) {
        auto val = x == y ? -1 : this->get(x, y);
        fmt::print("{:<3} ", val);
      }
      putchar('\n');
    }
  }

#define CHECK_STREAM(stream)                                              \
  if (stream.fail()) {                                                    \
    debug_printerr("Failed\n");                                           \
    return {};                                                            \
  }

  MSTMatrix
  MSTMatrix::buildFromFile(const char *filename)
  {
    std::ifstream is(filename);

    // Seems like each file starts with data name.
    std::string data_name;
    is >> data_name;
    CHECK_STREAM(is);

    index_type node_count;
    is >> node_count;
    CHECK_STREAM(is);

    MSTMatrix matrix(node_count);

    for (index_type y = 0; y < node_count; ++y) {
      for (index_type x = 0; x < node_count; ++x) {

        int64_t weight;
        is >> weight;
        CHECK_STREAM(is);

        if (weight == -1)
          weight = std::numeric_limits<decltype(weight)>::max();

        matrix.set(x, y, weight);
      }
    }

    return matrix;
  }

#undef CHECK_STREAM

} // namespace pea
