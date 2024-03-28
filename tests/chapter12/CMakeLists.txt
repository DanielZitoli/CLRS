# tests/chapter12/CMakeLists.txt

add_executable(test_tree test_binary_tree.cc)
target_link_libraries(test_tree PRIVATE gtest gtest_main chapter12_library tests_common_library)
