# benchmarks/CMakeLists.txt
add_executable(benchmark_hpgeom
  benchmark_gemm.cpp
  # add more benchmarks here
)

target_link_libraries(benchmark_hpgeom PRIVATE hpgeom benchmark::benchmark)
