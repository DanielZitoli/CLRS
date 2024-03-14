# src/chapter7/CMakeLists.txt

add_library(chapter7_library partitions.cc quick_sort.cc)

target_link_libraries(
  chapter7_library
  PRIVATE
  chapter5_library
)