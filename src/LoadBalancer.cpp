#include "LoadBalancer.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

LoadBalancer::LoadBalancer(int port, int numServers, int numThreadsPerServer) 
    : nextServer(0) {
    for (int i = 0; i < numServers; ++i) {
        servers.push_back(std::make_shared<Server>(port + i, numThreadsPerServer, [this](int clientSocket) {
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
    // Select a server to handle the request
    int selectedServer = getNextServer();
    
    // Pass the request to the selected server
    servers[selectedServer]->selectedServerToHandleRequest(clientSocket);
}

// Round robin selection of servers to handle a new request
int LoadBalancer::getNextServer() {
    int selectedServer = nextServer.fetch_add(1);
    
    // Wrap nextServer around when reaching the end
    selectedServer %= servers.size();
    
    return selectedServer;
}
