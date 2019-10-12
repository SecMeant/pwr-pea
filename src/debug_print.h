#pragma once

#include <fmt/format.h>
#include <fmt/printf.h>

#ifndef LOG_ERRORS
#define LOG_ERRORS 0

#define debug_print(...)
#define debug_printHL(...)
#define debug_printerr(...)
#define log_stdout(...)
#define log_stderr(...)

#else

template<typename... Ts>
decltype(auto)
log_print(Ts &&... args)
{
  fmt::print(std::forward<Ts>(args)...);
}

constexpr const char *LOG_DEF_FORMAT = "[{}][{}][L:{}] ";

#define debug_print(fmt, ...)                                             \
  {                                                                       \
    if constexpr (LOG_ERRORS) {                                           \
      log_print(LOG_DEF_FORMAT, __FILE__, __FUNCTION__, __LINE__);        \
      log_print(fmt, ##__VA_ARGS__);                                      \
    }                                                                     \
  }

#define debug_printHL(fmt, ...)                                           \
  {                                                                       \
    if constexpr (LOG_ERRORS) {                                           \
      log_print(fmt, ##__VA_ARGS__);                                      \
    }                                                                     \
  }

#define debug_printerr(fmt, ...)                                          \
  {                                                                       \
    if constexpr (LOG_ERRORS) {                                           \
      log_print(                                                          \
        stderr, LOG_DEF_FORMAT, __FILE__, __FUNCTION__, __LINE__);        \
      log_print(stderr, fmt, ##__VA_ARGS__);                              \
    }                                                                     \
  }

#define log_stdout(fmt, ...)                                              \
  {                                                                       \
    log_print(LOG_DEF_FORMAT, __FILE__, __FUNCTION__, __LINE__);          \
    log_print(fmt, ##__VA_ARGS__);                                        \
  }

#define log_stderr(fmt, ...)                                              \
  {                                                                       \
    log_print(stderr, LOG_DEF_FORMAT, __FILE__, __FUNCTION__, __LINE__);  \
    log_print(stderr, fmt, ##__VA_ARGS__);                                \
  }

#endif
