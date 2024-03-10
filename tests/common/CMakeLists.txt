# tests/common/CMakeLists.txt

add_library(tests_common_library sorting_fixture.cc)

target_link_libraries(
  tests_common_library
  PRIVATE
  gtest
  gtest_main
)