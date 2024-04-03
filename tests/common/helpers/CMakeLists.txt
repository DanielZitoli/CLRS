# tests/common/helpers/CMakeLists.txt

add_library(tests_common_library random_generators.cc printing_helpers.cc speed_profiler.cc)

target_link_libraries(
  tests_common_library
  PRIVATE
  gtest
  gtest_main
)
