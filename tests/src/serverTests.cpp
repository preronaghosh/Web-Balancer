#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "LoadBalancer.hpp"
#include "Logger.hpp"

class ServerTest : public testing::Test {
public:
    void SetUp() override {}

    static void requestHandlerMock(int clientSocket) {
        char buffer[1024] = {0};
        read(clientSocket, buffer, 1024);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    }

    // returns the response received by client
    static std::string createClientConnection(int serverPort) {
        int clientSocket;
        struct sockaddr_in serv_addr;
        if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cerr << "Socket creation error" << std::endl;
            return ""; 
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(serverPort);
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            close(clientSocket); 
            return ""; 
        }
        if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Connection Failed" << std::endl;
            close(clientSocket); 
            return ""; 
        }
        // Send a request
        std::string message = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
        send(clientSocket, message.c_str(), message.size(), 0);

        char buffer[1024] = {0};
        read(clientSocket, buffer, 1024);
        std::string response(buffer);

        close(clientSocket);
        return response; 
    }
};

/*********** SERVER TESTS ***********/

TEST_F(ServerTest, RateLimitingTest) {    
    // create a load balancer instance with 1 server and 4 threads/server
    LoadBalancer lb(9989, 1, 4);
    lb.start();

    // give server time to run
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Simulate 2 client connections
    auto response = createClientConnection(9989);
    ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    response = createClientConnection(9989);
    ASSERT_TRUE(response.find("429") != std::string::npos);
}

TEST_F(ServerTest, ServerRequestHandlingTest) {
    // Create a Server instance
    Server server(9999, 4, ServerTest::requestHandlerMock);
    server.start();

    // give server some time to start accepting connections 
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Create a socket and connect to the server
    auto response = createClientConnection(9999);
    ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
    server.stop();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}