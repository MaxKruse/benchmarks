cmake_minimum_required(VERSION 3.5)

SET(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
SET(CMAKE_BUILD_TYPE release)
SET(CMAKE_CXX_STANDARD 11)

project(Benchmarks)
find_package(benchmark CONFIG REQUIRED)

add_executable(main main.cpp StringUtils.cpp TimeUtils.cpp Map.cpp)
target_link_libraries(main PRIVATE benchmark::benchmark benchmark::benchmark_main)
