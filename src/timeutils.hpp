#pragma once
#include <chrono>
#include <fmt/format.h>
#include <functional>
#include <string>
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
    using namespace std::chrono_literals;

    constexpr auto tries = 50;
    auto total_time = 0ns;

    for (auto i = 0; i < tries; ++i) {
      total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(
        measure(std::forward<Function>(f), std::forward<Args>(args)...));
    }

    return total_time / tries;
  }

  template<typename Function, typename... Args>
  std::chrono::nanoseconds
  measure_milli(Function &&f, Args &&... args)
  {
    using namespace std::chrono_literals;

    constexpr auto tries = 50;
    auto total_time = 0ns;

    for (auto i = 0; i < tries; ++i) {
      total_time += std::chrono::duration_cast<std::chrono::milliseconds>(
        measure(std::forward<Function>(f), std::forward<Args>(args)...));
    }

    return total_time / tries;
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
  measure_and_log(FILE *f_out,
                  int psize,
                  const std::vector<std::string> &params,
                  Args &&... args)
  {
    auto time = measure_milli(std::forward<Args>(args)...);
    fmt::print(f_out,
               "{} & {} & {} \\\\\n",
               psize,
               time.count(),
               fmt::join(params, " &"));
  }
} // namespace pea
