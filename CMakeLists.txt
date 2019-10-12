project(pea CXX)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -fsanitize=address")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
project(sdizo CXX)

file(GLOB_RECURSE CXX_SOURCE src/*.cc)
add_executable(${PROJECT_NAME} ${CXX_SOURCE})
target_link_libraries(${PROJECT_NAME} stdc++fs fmt)

