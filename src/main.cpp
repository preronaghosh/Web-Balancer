#include "LoadBalancer.hpp"
#include "Logger.hpp"
#include <csignal>

void keyboardIntHandler(int signum) {
    LOGWARN("Shutting down server..");

    // Terminate the program
    exit(signum);
}

int main() {
    // Handle keyboard interrupt
    signal(SIGINT, keyboardIntHandler);

    LOGINFO("Starting a load balancer on port 8080..");
    LoadBalancer lb(8080, 4, std::thread::hardware_concurrency());
    lb.start();

    // Prevent the main thread from exiting
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}