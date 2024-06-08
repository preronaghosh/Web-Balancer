#!/bin/bash

# Parse command-line arguments
for arg in "$@"
do
    case $arg in
        --port=*)
        PORT="${arg#*=}"
        shift # Remove the argument and its value from the list
        ;;
        *)
        # Unknown option
        echo "Unknown option: $arg"
        exit 1
        ;;
    esac
done

# Validate port number
if ! [[ "$PORT" =~ ^[0-9]+$ ]]; then
    echo "Invalid port number: $PORT"
    exit 1
fi

# Cleanup previous build dir
rm -rf build/
mkdir build/
cd build/

# Build
cmake ..
make

# Run the generated executable with the extracted port number
./main $PORT
