ROOT_DIR=$(pwd)

# Function to build the project with CMake
build_cmake() {
    # Create a build directory if it doesn't exist
    mkdir -p "$ROOT_DIR/build"
    cd "$ROOT_DIR/build" || exit 1

    # Run CMake to generate build files
    cmake ..

    # Return to the root directory
    cd "$ROOT_DIR" || exit 1
}

# Function to run make in the build directory
run_make() {
    cd "$ROOT_DIR/build" || exit 1

    # Run make to build the project
    make

    # Return to the root directory
    cd "$ROOT_DIR" || exit 1
}

# Finds executable with the given chaper and test name in the 
# /build directory and runs the test
run_test() {
    if [ "$#" -lt 2 ]; then
        echo "Usage: run_test [chapter] [test_name]"
        return 1
    fi

    section_name=$1
    test_name=$2
    test_file="build/tests/${section_name}/test_${test_name}"

    if [ -f "$test_file" ]; then
        echo "Running test: $test_name"
        ./"$test_file"
    else
        echo "Test file not found: $test_file"
        return 1
    fi
}