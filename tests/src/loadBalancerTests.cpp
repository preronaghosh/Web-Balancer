#include "tests_common.hpp"

class LoadBalancerTests : public WebServerTests {
public:
    void SetUp() override {}
};

/*********** LOAD BALANCER TESTS ***********/

TEST_F(LoadBalancerTests, RoundRobinSchedulingTest) {
    // lb with 3 servers and 4 worker threads per server
    LoadBalancer lb(9934, 3, 4);
    lb.start();

    std::string response{""}; 
    for (int i=0; i<4; i++) {
        std::cout << "Sending request " << i << " to server 9934 " << std::endl; 
        response = createClientConnection(9934);
        ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    for (int i=0; i<4; i++) {
        std::cout << "Sending request " << i << " to server 9935 " << std::endl; 
        response = createClientConnection(9935);
        ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    for (int i=0; i<4; i++) {
        std::cout << "Sending request " << i << " to server 9936 " << std::endl; 
        response = createClientConnection(9936);
        ASSERT_TRUE(response.find("HTTP/1.1 200 OK") != std::string::npos);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}