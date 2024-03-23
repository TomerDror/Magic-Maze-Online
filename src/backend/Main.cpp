#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

const char* SERVER_IP = "127.0.0.1"; // Change this to the server's IP address
const int SERVER_PORT = 27015;
const int BUFFER_SIZE = 1024;

void HandleServerMessages(SOCKET serverSocket) {
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while (true) {
        bytesReceived = recv(serverSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Error receiving from server. Connection closed." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received from server: " << buffer << std::endl;
        // Here you can pass the received message to your other thread for processing
    }
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating client socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    result = connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // Start a thread to handle server messages
    std::thread serverThread(HandleServerMessages, clientSocket);

    // Main loop to send commands to the server
    while (true) {
        std::string message;
        std::cout << "Enter command: ";
        std::getline(std::cin, message);

        if (send(clientSocket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            std::cerr << "Error sending message to server: " << WSAGetLastError() << std::endl;
            break;
        }

        if (message == "quit") {
            break;
        }
    }

    // Close the socket and join the server thread
    closesocket(clientSocket);
    serverThread.join();

    WSACleanup();
    return 0;
}
