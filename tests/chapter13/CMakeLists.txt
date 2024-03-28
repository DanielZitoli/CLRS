# tests/chapter13/CMakeLists.txt

add_executable(test_rb_tree test_rb_tree.cc)
target_link_libraries(test_rb_tree PRIVATE gtest gtest_main chapter13_library tests_common_library)
