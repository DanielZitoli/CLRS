# tests/chapter6/CMakeLists.txt

add_executable(test_heaps test_heaps.cc)
add_executable(test_heap_based_structs test_heap_based_structs.cc)
target_link_libraries(test_heaps PRIVATE gtest gtest_main chapter6_library tests_common_library)
target_link_libraries(test_heap_based_structs PRIVATE gtest gtest_main chapter6_library tests_common_library)
