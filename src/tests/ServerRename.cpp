#include <iostream>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <winsock2.h>
// #pragma comment(lib, "ws2_32.a")
#else
#include <unistd.h>
#include <arpa/inet.h>
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
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket" << std::endl;
        closesocket(serverSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Error listening for connections" << std::endl;
        closesocket(serverSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // Accept a connection
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error accepting connection" << std::endl;
        closesocket(serverSocket);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return EXIT_FAILURE;
    }

    std::cout << "Connection accepted. Echoing messages..." << std::endl;

    // Echo messages back to the client
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    // Close the sockets and cleanup on Windows
    closesocket(clientSocket);
    closesocket(serverSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
