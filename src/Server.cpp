#include "Server.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port, int workerThreads, std::function<void(int)> requestHandler)
    : port(port), threadPool(workerThreads), requestHandler(requestHandler) {}


void Server::start() {
    std::thread(&Server::acceptConnections, this).detach();
}

void Server::selectedServerToHandleRequest(int clientSocket) {
    threadPool.enqueueTask([clientSocket]() {
        char buffer[1024] = {0};
        read(clientSocket, buffer, 1024);
        std::cout << "Received a request: " << buffer << std::endl;
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    });
}

void Server::acceptConnections() {
    int serverFd, newSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket failed" << std::endl;
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    if (listen(serverFd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }

    while (true) {
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            return;
        }

        if (requestHandler) {
            requestHandler(newSocket);
        } else {
            std::cerr << "Request handler not set" << std::endl;
            close(newSocket);
        }
    }
}
