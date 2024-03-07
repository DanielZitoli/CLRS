# tests/chapter5/CMakeLists.txt

add_executable(test_random test_random.cc)
target_link_libraries(test_random PRIVATE gtest gtest_main chapter5_library)
