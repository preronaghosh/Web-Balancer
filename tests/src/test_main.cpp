#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

class ServerTest : public testing::Test {
public:
    void SetUp() override {}
};

/***** MOCK FUNCTIONS *****/
void requestHandler(int clientSocket) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    send(clientSocket, response.c_str(), response.size(), 0);
    close(clientSocket);
}

/*********** SERVER TESTS ***********/

TEST_F(ServerTest, DISABLED_RateLimitingTest) {
    Server server(8080, 1, requestHandler);
    server.setRateLimit(1, std::chrono::seconds(4));
    std::thread serverThread([&server]{ 
        server.start(); 
        std::this_thread::sleep_for(std::chrono::seconds(10));
    });

    // Simulate multiple client connections
    std::vector<std::thread> clientThreads;
    for (int i = 0; i < 50; ++i) {
        clientThreads.emplace_back([]() {
            int clientSocket;
            struct sockaddr_in serv_addr;
            if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                std::cerr << "Socket creation error" << std::endl;
                return;
            }
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(8080);
            if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
                std::cerr << "Invalid address/ Address not supported" << std::endl;
                return;
            }
            if (connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                std::cerr << "Connection Failed" << std::endl;
                return;
            }
            // Send a request
            std::string message = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
            send(clientSocket, message.c_str(), message.size(), 0);
            close(clientSocket);
        });
    }

    // Allow some time for rate limiting to take effect
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Try to establish another connection after the rate limit window has passed

    int clientSocket;
    struct sockaddr_in serv_addr;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return;
    }
    ASSERT_EQ(connect(clientSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)), 0);
    // Send a request
    std::string message = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(clientSocket, message.c_str(), message.size(), 0);
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    // Check if the server responds with "429 Too Many Requests"
    std::string response(buffer);
    std::cout << response << std::endl;
    EXPECT_TRUE(response.find("Too Many Requests") != std::string::npos);

    // Wait for client threads to finish
    for (auto& thread : clientThreads) {
        thread.join();
    }

    serverThread.join();
    
    // Stop the server
    server.stop();
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}