# src/chapter9/CMakeLists.txt

add_library(chapter9_library order_statistics.cc)

target_link_libraries(
  chapter9_library
  PRIVATE
  chapter5_library
)
