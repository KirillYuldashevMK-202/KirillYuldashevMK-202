#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return EXIT_FAILURE;
    }
#endif

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(server, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Bind failed" << std::endl;
        return EXIT_FAILURE;
    }

    if (listen(server, 1) == -1) {
        std::cerr << "Listen failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port 12345" << std::endl;

    sockaddr_in clientAddr{};
    int clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(server, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Accept failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connection accepted" << std::endl;

    char buffer[1024];
    int messageCounter = 0;
    int totalMessagesReceived = 0;

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            std::cerr << "Receive failed" << std::endl;
            break;
        }

        if (strlen(buffer) == 0) {
            std::cout << "Connection closed by client" << std::endl;
            break;
        }

        totalMessagesReceived++;
        std::cout << "Received message from client: " << buffer << std::endl;

        sprintf_s(buffer, "%s TotalMessagesReceived: %d\n", buffer, ++messageCounter, totalMessagesReceived);
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            std::cerr << "Send failed" << std::endl;
            break;
        }

        std::cout << "Sent response to client: " << buffer << std::endl;
    }

#ifdef _WIN32
    closesocket(server);
    closesocket(clientSocket);
    WSACleanup();
#else
    close(serverSocket);
    close(clientSocket);
#endif

    return 0;
}
