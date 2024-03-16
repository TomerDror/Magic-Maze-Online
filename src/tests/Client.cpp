#include <iostream>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
// #pragma comment(lib, "ws2_32.a")
#else
#include <unistd.h>
#include <arpaf/inet.h>
#endif

int main() {
    // Initialize Winsock on Windows
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return EXIT_FAILURE;
    }
    #endif

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(8080);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server" << std::endl;
        closesocket(clientSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server. Type a message to send ('exit' to quit):" << std::endl;

    // Send messages to the server
    char message[1024];
    while (true) {
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "exit") == 0) {
            break;
        }

        send(clientSocket, message, strlen(message), 0);

        // Receive and print the echoed message
        char buffer[1024];
        recv( clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Server echoed: " << buffer << std::endl;
    }

    // Close the socket and cleanup on Windows
    closesocket(clientSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
