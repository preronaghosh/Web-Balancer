#include "tests_common.hpp"

std::string WebServerTests::createClientConnection(int serverPort) {
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