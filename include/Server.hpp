#ifndef SERVER_HPP
#define SERVER_HPP

#include <functional>
#include <thread>

class Server {
public:
    Server(int port, std::function<void(int)> requestHandler);
    void start();

private:
    int port;
    std::function<void(int)> requestHandler;
    void acceptConnections();
};

#endif // SERVER_HPP
