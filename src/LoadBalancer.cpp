#include "LoadBalancer.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

LoadBalancer::LoadBalancer(int port, int numServers, int numThreads) 
    : threadPool(numThreads), nextServer(0) {
    for (int i = 0; i < numServers; ++i) {
        servers.push_back(std::make_shared<Server>(port + i, [this](int clientSocket) {
            this->handleRequest(clientSocket);
        }));
    }
}

void LoadBalancer::start() {
    for (auto& server : servers) {
        server->start();
    }
}

void LoadBalancer::handleRequest(int clientSocket) {
    threadPool.enqueueTask([clientSocket]() {
        char buffer[1024] = {0};
        read(clientSocket, buffer, 1024);
        std::cout << "Request: " << buffer << std::endl;
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    });
}
