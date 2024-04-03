# tests/common/fixtures/CMakeLists.txt

add_library(tests_sorting_library sorting_fixture.cc)

target_link_libraries(
  tests_sorting_library
  PRIVATE
  gtest
  gtest_main
  tests_common_library
)