cmake_minimum_required(VERSION 3.16)
project(hpgeom LANGUAGES CXX)

# -------------------------------------------------------------------
# Project-wide settings
# -------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Export compile commands for editor tooling (clangd / IntelliSense)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Allow user to choose build type, default to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler flags for performance builds (tweak later as needed)
if (MSVC)
  add_compile_options(/W4 /MP)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
  # -march=native is handy locally; remove or guard in CI if needed
  add_compile_options(-march=native)
endif()

option(HPGEOM_ENABLE_BENCHMARKS "Enable building benchmarks" OFF)

# -------------------------------------------------------------------
# Library target
# -------------------------------------------------------------------
add_library(hpgeom STATIC
  src/csr.cpp
  src/blocked_matrix.cpp
  # add other implementation files here
)

target_include_directories(hpgeom
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_definitions(hpgeom PRIVATE HPGEOM_INTERNAL)

# -------------------------------------------------------------------
# Enable testing early so add_test() calls register correctly
# -------------------------------------------------------------------
enable_testing()

# -------------------------------------------------------------------
# Tests (GoogleTest via FetchContent)
# -------------------------------------------------------------------
include(FetchContent)

# GoogleTest - set DOWNLOAD_EXTRACT_TIMESTAMP to avoid CMP0135 warning
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# On MSVC we prefer shared CRT for gtest to match app CRT; harmless on other platforms
if (MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Add tests subdirectory (tests should call add_executable / add_test)
add_subdirectory(tests)

# -------------------------------------------------------------------
# Optional benchmarks (Google Benchmark)
# -------------------------------------------------------------------
if(HPGEOM_ENABLE_BENCHMARKS)
  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.7.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  FetchContent_MakeAvailable(googlebenchmark)
  add_subdirectory(benchmarks)
endif()

# -------------------------------------------------------------------
# Install rules (optional)
# -------------------------------------------------------------------
install(TARGETS hpgeom
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)
