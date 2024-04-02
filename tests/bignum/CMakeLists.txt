# tests/bignum/CMakeLists.txt

add_executable(test_big_int test_big_int.cc)
target_link_libraries(test_big_int PRIVATE gtest gtest_main bignum_library tests_common_library)
