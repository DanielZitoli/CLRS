# tests/chapter10/CMakeLists.txt

add_executable(test_list test_list.cc)
target_link_libraries(test_list PRIVATE gtest gtest_main chapter10_library tests_common_library)
