#ifndef LOADBALANCER_HPP
#define LOADBALANCER_HPP

#include <vector>
#include <memory>
#include <thread>
#include "Server.hpp"
#include "ThreadPool.hpp"

/**
 * @brief The LoadBalancer class represents a TCP load balancer that distributes incoming client connections 
 * across multiple server instances.
 * 
 * This class manages a pool of server instances, each listening on a different port, and utilizes a thread pool
 * for parallel request processing. It balances the load of incoming client connections across the server instances
 * to improve system scalability and performance.
 * 
 * @note This class uses a shared thread pool for request processing, allowing for efficient handling of 
 * multiple client connections concurrently.
 */

class LoadBalancer {
public:

    /**
     * @brief Constructs a new LoadBalancer object with the specified parameters.
     * 
     * @param port The base port number on which the server instances will listen for incoming connections.
     * @param numServers The number of server instances to create for load balancing.
     * @param numThreads The number of worker threads in the thread pool for request processing.
     */
    LoadBalancer(int port, int numServers, int numThreads);

    /**
     * @brief Starts the load balancer and all associated server instances.
     * 
     * This method starts all server instances created by the load balancer, allowing them to accept incoming
     * client connections and delegate them to the thread pool for request processing.
     */
    void start();

private:
    std::vector<std::shared_ptr<Server>> servers; ///< The pool of server instances managed by the load balancer.
    ThreadPool threadPool; ///< The thread pool for parallel request processing.
    int nextServer; ///< Index of the next server instance to which a client connection should be directed.

    /**
     * @brief Handles an incoming client connection by enqueueing it into the thread pool for processing.
     * 
     * @param clientSocket The socket file descriptor representing the client connection.
     * 
     * This method enqueues the client socket into the thread pool, allowing it to be processed asynchronously
     * by one of the worker threads.
     */
    void handleRequest(int clientSocket);
};

#endif // LOADBALANCER_HPP
