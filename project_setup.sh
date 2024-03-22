ROOT_DIR=$(pwd)

alias c=clear
HISTCONTROL=ignoredups:erasedups

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

        exit_status=$?
        if [ $exit_status -eq 0 ]; then
            echo "Test $test_name passed"
        else
            echo "Test $test_name failed with exit code $exit_status"
        fi
        return $exit_status
    else
        echo "Test file not found: $test_file"
        return 2
    fi
}

# Runs all tests given by a file name, or by default it checks test_names.txt
run_all_tests() {
    if [ "$#" -lt 1 ]; then
        test_case_file="test_names.txt"
    else 
        test_case_file=$1
    fi

    echo "Running all tests..."
    failed_tests=()
    while read -r test; do
        section_name=$(echo "$test" | cut -d ' ' -f 1)
        test_name=$(echo "$test" | cut -d ' ' -f 2)
        run_test "$section_name" "$test_name"
        exit_status=$?
        if [ $exit_status -ne 0 ]; then
            failed_tests+=("$section_name $test_name --- Exit Code: $exit_status")
        fi
    done < "$test_case_file"
    
    echo -e "\n"
    echo "##### #   # #   # #   #  ###  ####  #   #  "
    echo "#     #   # #\ /# #\ /# #   # #   #  # #   "
    echo "##### #   # # # # # # # ##### ####    #    "
    echo "    # #   # #   # #   # #   # #  #    #    "
    echo "##### ##### #   # #   # #   # #   #   #    "
    echo -e "\n"
    
    if [ ${#failed_tests[@]} -eq 0 ]; then
        echo "All tests passed!!!"
    else
        echo "Failed tests:"
        printf '%s\n' "${failed_tests[@]}"
    fi
}