# Multithreaded Web Server with Load Balancer

A simple multithreaded web server with a load balancer implemented in C++.

## Features

- Handles multiple client requests concurrently.
- Distributes requests across multiple servers.
- Uses a thread pool for efficient task execution.

## How to Build

```bash
cmake ..
```

## Example Request

```bash
cd build/
curl http://localhost:8080
```

The server will respond with a "Hello, World!" message.