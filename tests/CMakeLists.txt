# tests/CMakeLists.txt

# Include Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Function to automate linking a specific testing file to it's chapter's library
function(configure_test_executable target_name section_name)
  add_executable(${target_name} test_${target_name}.cc)
  target_link_libraries(${target_name} PRIVATE gtest gtest_main tests_common_library ${section_name}_library)
endfunction()

# Include chapter tests
add_subdirectory(common)
add_subdirectory(chapter2)
add_subdirectory(chapter5)
add_subdirectory(chapter6)
add_subdirectory(chapter7)
add_subdirectory(chapter8)
add_subdirectory(chapter9)
add_subdirectory(chapter10)
add_subdirectory(chapter11)
add_subdirectory(chapter12)
add_subdirectory(chapter13)
