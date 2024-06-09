# Web-Balancer

A simple multithreaded web server with a load balancer implemented in C++.

## Features

- Handles multiple client requests concurrently.
- Distributes requests across multiple servers.
- Uses a thread pool for efficient task execution.

## Manual and Automated Build Scripts

### Manual Build

```bash
mkdir build/
cd build/
cmake .. && make
```

### Build Script

```bash
./build.sh 
```

### Script for running tests after building

```bash
./run_tests.sh 
```

### Build and execute program
```bash
./run_server.sh --port=1234 --servers=4
```


## Example Request

```bash
curl http://localhost:1234
```

The server will respond with a "Hello, World!" message.