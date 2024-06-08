#include <csignal>
#include <string>   
#include <stdexcept>
#include <cstdlib>
#include "LoadBalancer.hpp"
#include "Logger.hpp"


void keyboardIntHandler(int signum) {
    LOGWARN("Shutting down server..");

    // Terminate the program
    exit(signum);
}

int main(int argc, char* argv[]) {
    // Handle keyboard interrupt
    signal(SIGINT, keyboardIntHandler);
    int port = 8080; // Default port
    int numServers = 1; // Default number of servers

    if (argc > 1) {
        try {
            for (int i = 1; i < argc; ++i) {
                std::string arg = argv[i];
                if (arg.find("--port=") == 0) {
                    port = std::stoi(arg.substr(7));
                } else if (arg.find("--servers=") == 0) {
                    numServers = std::stoi(arg.substr(10));
                }
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument provided: " << e.what() << std::endl;
            return 1; 
        } catch (const std::out_of_range& e) {
            std::cerr << "Argument out of range: " << e.what() << std::endl;
            return 1; 
        }
    }

    LOGINFO("Starting a load balancer..");
    LoadBalancer lb(port, numServers, std::thread::hardware_concurrency());
    lb.start();

    // Prevent the main thread from exiting
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}