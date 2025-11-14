# tests/CMakeLists.txt
add_executable(test_hpgeom
  test_main.cpp
  test_csr.cpp
  # add more unit test files here
)

target_link_libraries(test_hpgeom PRIVATE hpgeom gtest_main)

# Add common compile options for tests if needed
if (NOT MSVC)
  target_compile_options(test_hpgeom PRIVATE -O0 -g)
endif()

add_test(NAME unit_tests COMMAND test_hpgeom)
