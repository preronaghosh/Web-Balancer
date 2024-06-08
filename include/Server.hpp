#ifndef SERVER_HPP
#define SERVER_HPP

#include <functional>
#include <thread>

/**
 * @brief The Server class represents a TCP server that listens for incoming connections on a specified port.
 * 
 * This class encapsulates the functionality of a TCP server, allowing it to accept incoming client connections
 * and delegate the handling of these connections to a user-defined request handler function.
 * 
 * @note This class operates in a single-threaded manner, handling each client connection sequentially.
 */

class Server {
public:
    /**
     * @brief Constructs a new Server object with the specified port and request handler function.
     * 
     * @param port The port number on which the server will listen for incoming connections.
     * @param requestHandler The function to handle incoming client connections. This function
     * will be called with the client socket file descriptor as its argument.
     */
    Server(int port, std::function<void(int)> requestHandler);

    /**
     * @brief Starts the server, allowing it to accept incoming connections.
     * 
     * This method starts the server's main loop, accepting incoming client connections
     * and delegating them to the request handler function.
     */
    void start();

private:
    int port; ///< The port number on which the server listens for incoming connections.
    std::function<void(int)> requestHandler; ///< The function to handle incoming client connections.

    /**
     * @brief Accepts incoming client connections and delegates them to the request handler function.
     * 
     * This method continuously accepts incoming client connections on the server's listening port.
     * For each accepted connection, it calls the request handler function to process the client request.
     */
    void acceptConnections();
};

#endif // SERVER_HPP
