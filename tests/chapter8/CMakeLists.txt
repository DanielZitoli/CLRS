# tests/chapter8/CMakeLists.txt

add_executable(test_linear_sorting test_linear_sorting.cc)
target_link_libraries(test_linear_sorting PRIVATE gtest gtest_main chapter5_library chapter8_library tests_common_library)
