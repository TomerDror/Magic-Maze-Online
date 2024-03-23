#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <winsock2.h>
#include <WS2tcpip.h>

const int PORT = 8089;
const std::string IP = "127.0.0.1";
const std::string START_COMMAND = "start";

void receiveCommands(int clientSocket) {
    while (true) {
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error: Connection closed by server." << std::endl;
            closesocket(clientSocket);
            return;
        }
        buffer[bytesReceived] = '\0';
        std::string message(buffer);
        std::cout << "Received from server: " << message << std::endl;

        if (message == START_COMMAND) {
            std::cout << "Client started!" << std::endl;
        }
    }
}

void sendCommands(int clientSocket) {
    std::string cmd;
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        if (cmd == "stop") {
            send(clientSocket, "stop", 4, 0); // Inform server to stop
            break;
        }
        send(clientSocket, cmd.c_str(), cmd.size(), 0);
    }
}
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

    // Accept connections and handle them in separate threads
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // Create a new thread to handle the client
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Detach the thread to run independently
    }

    // Close the server socket and cleanup on Windows
    closesocket(serverSocket);
    #ifdef _WIN32
    WSACleanup();
    #endif

    return 0;
}
