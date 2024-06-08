#!/bin/bash

# Parse command-line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --port=*)
        PORT="${1#*=}"
        shift
        ;;
        --servers=*)
        SERVERS="${1#*=}"
        shift
        ;;
        *)
        # Unknown option
        echo "Unknown option: $1"
        exit 1
        ;;
    esac
done

# Validate port number
if ! [[ "$PORT" =~ ^[0-9]+$ ]]; then
    echo "Invalid port number: $PORT"
    exit 1
fi

# Validate number of servers
if ! [[ "$SERVERS" =~ ^[0-9]+$ ]]; then
    echo "Invalid number of servers: $SERVERS"
    exit 1
fi

# Cleanup previous build dir
rm -rf build/
mkdir build/
cd build/

# Build
cmake ..
make

# Run the generated executable with the extracted port number and number of servers
./main --port="$PORT" --servers="$SERVERS"