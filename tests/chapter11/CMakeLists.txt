# tests/chapter11/CMakeLists.txt

add_executable(test_hashing test_hashing.cc)
add_executable(test_hash_table test_hash_table.cc)
add_executable(test_open_addressing test_open_addressing.cc)
add_executable(test_perfect_hashing test_perfect_hashing.cc)
target_link_libraries(test_hashing PRIVATE gtest gtest_main chapter11_library tests_common_library)
target_link_libraries(test_hash_table PRIVATE gtest gtest_main chapter11_library tests_common_library)
target_link_libraries(test_open_addressing PRIVATE gtest gtest_main chapter11_library tests_common_library)
target_link_libraries(test_perfect_hashing PRIVATE gtest gtest_main chapter11_library tests_common_library)
