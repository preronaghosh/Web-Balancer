#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Logger.hpp"

int Server::maxRequestsPerIp;
std::chrono::seconds Server::rateLimitDuration;

Server::Server(int port, int workerThreads, std::function<void(int)> requestHandler)
    : port{port}, 
      threadPool(workerThreads), 
      requestHandler{requestHandler}, 
      currState{ServerState::STARTING},
      serverFd{-1} {

    // set rate limits
    this->setRateLimit(1, std::chrono::seconds{4});
}


void Server::start() {
    setServerState(ServerState::STARTING);
    std::thread(&Server::acceptConnections, this).detach();
}

// Not called by anyone currently
void Server::stop() {
    setServerState(ServerState::SHUTTING_DOWN);
    if (serverFd != -1) {
        close(serverFd);
        serverFd = -1;
    }
    setServerState(ServerState::STOPPED);
}

void Server::selectedServerToHandleRequest(int clientSocket) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    getpeername(clientSocket, (struct sockaddr*)&addr, &addrLen);
    std::string clientIp = inet_ntoa(addr.sin_addr);

    // Reject request if it beyond the rate limits set
    if (isRateLimited(clientIp)) {
        std::string response = "HTTP/1.1 429 Too Many Requests\r\nContent-Length: 21\r\n\r\nToo Many Requests";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
        return;
    }

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

    setServerState(ServerState::ACTIVE);

    while (getServerState() == ServerState::ACTIVE) {
        int newSocket;
        if ((newSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            if (getServerState() == ServerState::SHUTTING_DOWN) {
                break; // Exit accept loop if shutting down
            }
            std::cerr << "Accept failed" << std::endl;
            return;
        }

        if (requestHandler) {
            setServerState(ServerState::SERVING);
            requestHandler(newSocket);
            setServerState(ServerState::ACTIVE);
        } else {
            std::cerr << "Request handler not set" << std::endl;
            close(newSocket);
            setServerState(ServerState::INACTIVE);
        }
    }
}

void Server::setRateLimit(int requests, std::chrono::seconds duration) {
    maxRequestsPerIp = requests;
    rateLimitDuration = duration;
}

bool Server::isRateLimited(const std::string& clientIp) {
    auto currentTime = std::chrono::steady_clock::now();
    auto& requestInfo = requestCountsPerIp[clientIp];

    if (currentTime - requestInfo.second > rateLimitDuration) {
        // not within the time window
        requestInfo.first.store(1);
        requestInfo.second = currentTime;
    } else {
        // increment request count if within the time window
        requestInfo.first++;
    }
    return requestInfo.first > maxRequestsPerIp;
}
