project(pea CXX)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
  option(LOG_ERRORS "Enables or disables error logging to stderr by libmipc" ON)
else()
  option(LOG_ERRORS "Enables or disables error logging to stderr by libmipc" OFF)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -fsanitize=address")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fno-exceptions")

if(LOG_ERRORS)
  add_compile_definitions(LOG_ERRORS=1)
endif()

file(GLOB_RECURSE CXX_SOURCE src/*.cc)
add_executable(${PROJECT_NAME} ${CXX_SOURCE})
target_link_libraries(${PROJECT_NAME} stdc++fs fmt)

