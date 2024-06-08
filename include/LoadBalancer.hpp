#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include <vector>
#include <memory>
#include <thread>
#include "Server.hpp"
#include "ThreadPool.hpp"

class LoadBalancer {
public:
    LoadBalancer(int port, int numServers, int numThreads);
    void start();

private:
    std::vector<std::shared_ptr<Server>> servers;
    ThreadPool threadPool;
    int nextServer;

    void handleRequest(int clientSocket);
};

#endif // LOADBALANCER_HPP
