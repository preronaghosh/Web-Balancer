#include "tests_common.hpp"

class ServerTest : public WebServerTests {
public:
    void SetUp() override {}

    static void requestHandlerMock(int clientSocket) {
        char buffer[1024] = {0};
        read(clientSocket, buffer, 1024);
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
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