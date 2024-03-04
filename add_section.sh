#!/bin/bash

# Function to generate CMakeLists.txt for src directory
generate_src_CMakeLists() {
    local section_name=$1
    shift
    local files=("${@}")

    echo "Generating CMakeLists.txt for src/$section_name directory..."

    # Add comment with filename at the top 
    echo -e "# src/$section_name/CMakeLists.txt\n" >> src/$section_name/CMakeLists.txt

    # Add add_library commands for each file
    for file in "${files[@]}"; do
      echo "add_library(${section_name}_${file}_library)" >> src/$section_name/CMakeLists.txt
    done
}

# Function to generate CMakeLists.txt for test directory
generate_test_CMakeLists() {
    local section_name=$1
    shift
    local files=("${@}")

    echo "Generating CMakeLists.txt for test directory..."

    # Add comment with filename at the top 
    echo -e "# tests/$section_name/CMakeLists.txt\n" >> tests/$section_name/CMakeLists.txt

    # Add add_library commands for each file
    for file in "${files[@]}"; do
      echo "add_library(test_${section_name}_${file})" >> tests/$section_name/CMakeLists.txt
    done

    # Add linking commands for each file
    for file in "${files[@]}"; do
      echo "target_link_libraries(test_${section_name}_${file})" >> tests/$section_name/CMakeLists.txt
    done
}

# Function to create directory structure and files for new section
create_section() {
    local section_name=$1
    shift
    local files=("${@}")

    echo "Creating directory structure for section '$section_name'..."

    echo "Files:"
    echo ${files[@]}

    # Create directories for section in include, src, and tests
    mkdir -p include/$section_name
    mkdir -p src/$section_name
    mkdir -p tests/$section_name


    # Generate .h files in include directory
    echo "Generating .h files for section '$section_name'..."
    # for file in "${files[@]}"; do
    #   touch include/$section_name/${file}.h
    # done

    # Generate .cc files in src directory
    echo "Generating .cc files for section '$section_name'..."
    # for file in "${files[@]}"; do
    #   touch src/$section_name/${file}.cc
    # done

    # Generate test_name.cc files in test directory for each executable
    echo "Generating test files for section '$section_name'..."
    # for file in "${files[@]}"; do
    #   touch tests/$section_name/test_${file}.cc
    # done

    # Add add_subdirectory($section_name) command to src/CMakeLists.txt 
    echo "Appending add_subdirectory($section_name) to src/CMakeLists.txt..."
    # echo "add_subdirectory($section_name)" >> src/CMakeLists.txt

    # Add add_subdirectory($section_name) command to tests/CMakeLists.txt 
    echo "Appending add_subdirectory($section_name) to tests/CMakeLists.txt..."
    # echo "add_subdirectory($section_name)" >> tests/CMakeLists.txt


    # Generate CMakeLists.txt for src directory
    generate_src_CMakeLists "$section_name" "${files[@]}"

    # Generate CMakeLists.txt for test directory
    generate_test_CMakeLists "$section_name" "${files[@]}"

    echo "New section '$section_name' created successfully."
}

# Execute main function with provided arguments
create_section "$@"
