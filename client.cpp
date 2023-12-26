#include <iostream>
#include <cstring>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
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

    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(12345);

    if (connect(client, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Connection failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server" << std::endl;

    char message[1024];
    char buffer[1024];

    while (true) {
        std::cout << "Enter your message: ";
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "disconnect") == 0) {
            break;
        }

        if (send(client, message, strlen(message), 0) == -1) {
            std::cerr << "Failed" << std::endl;
            break;
        }

        std::cout << "Message to server: " << message << std::endl;

        memset(buffer, 0, sizeof(buffer));

        if (recv(client, buffer, sizeof(buffer), 0) == -1) {
            std::cerr << "Failed" << std::endl;
            break;
        }

        std::cout << "Received response from server: " << buffer << std::endl;
    }

#ifdef _WIN32
    closesocket(client);
    WSACleanup();
#else
    close(clientSocket);
#endif

    return 0;
}
