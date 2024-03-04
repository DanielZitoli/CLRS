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