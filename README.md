# Multithreaded Web Server with Load Balancer

A simple multithreaded web server with a load balancer implemented in C++.

## Features

- Handles multiple client requests concurrently.
- Distributes requests across multiple servers.
- Uses a thread pool for efficient task execution.

## How to Build Manually

```bash
mkdir build/
cd build/
cmake ..
make
./main
```

## Run servers with script
```bash
./run_server.sh --port=1234 
```

## Example Request

```bash
curl http://localhost:8080
```

The server will respond with a "Hello, World!" message.