#include <csignal>
#include <string>    
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
    int port;

    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid port number provided: " << argv[1] << std::endl;
            return 1; 
        } catch (const std::out_of_range& e) {
            std::cerr << "Port number out of range: " << argv[1] << std::endl;
            return 1; 
        }
    }

    LOGINFO("Starting a load balancer..");
    LoadBalancer lb(port, 4, std::thread::hardware_concurrency());
    lb.start();

    // Prevent the main thread from exiting
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}