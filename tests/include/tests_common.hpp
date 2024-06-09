#include <gtest/gtest.h>
#include <thread>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "LoadBalancer.hpp"
#include "Logger.hpp"


class WebServerTests : public testing::Test {
public:
    void SetUp() override {}

    static std::string createClientConnection(int serverPort);
};
