# tests/chapter2/CMakeLists.txt

add_executable(test_chapter2 test_insertion_sort.cc)
add_executable(test_searching test_searching.cc)
add_executable(test_sorting test_sorting.cc) 

target_link_libraries(
  test_chapter2
  PRIVATE
  gtest
  gtest_main
  chapter2_library
)

target_link_libraries(
  test_searching
  PRIVATE
  gtest
  gtest_main
  chapter2_searching_library
)

target_link_libraries(
  test_sorting
  PRIVATE
  gtest
  gtest_main
  chapter2_sorting_library
)
