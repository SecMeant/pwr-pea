#pragma once
#include <chrono>
#include <fmt/format.h>
#include <functional>
#include <utility>

namespace pea {
  template<typename Function, typename... Args>
  decltype(auto)
  measure(Function &&f, Args &&... args) noexcept
  {
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<Function>(f), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    return end - start;
  }

  template<typename Function, typename... Args>
  std::chrono::nanoseconds
  measure_nano(Function &&f, Args &&... args)
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
      measure(std::forward<Function>(f), std::forward<Args>(args)...));
  }

  template<typename DataStructure, typename Operation, typename... Args>
  std::chrono::nanoseconds
  measure_operation_nano(int32_t initial_size,
                         DataStructure &&data_structure,
                         Operation &&op,
                         Args &&... args)
  {
    std::invoke(&std::remove_reference_t<DataStructure>::generate,
                std::forward<DataStructure>(data_structure),
                0,
                100,
                initial_size);
    return measure_nano(std::forward<Operation>(op),
                        std::forward<DataStructure>(data_structure),
                        std::forward<Args>(args)...);
  }

  // Doing measurements and logs results.
  // passes args to measure_operation_nano.
  template<typename... Args>
  void
  measure_and_log(const char *m_name, const char *f_name, Args &&... args)
  {
    FILE *f_out = fopen(f_name, "a");
    auto time = measure_nano(std::forward<Args>(args)...);
    fmt::print(f_out, "{};{};\n", m_name, time.count());
    fclose(f_out);
  }
} // namespace pea
