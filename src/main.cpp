#include "LoadBalancer.hpp"

int main() {
    LoadBalancer lb(8080, 4, std::thread::hardware_concurrency());
    lb.start();

    // Prevent the main thread from exiting
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }

    return 0;
}