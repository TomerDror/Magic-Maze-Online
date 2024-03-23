#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstring>
#include <winsock2.h>
#include <windows.h>
#include <algorithm> // for std::remove

const int PORT = 8089;
const int MAX_CLIENTS = 2;
const std::string START_COMMAND = "start";

std::vector<int> clients;
std::mutex clientsMutex;
std::atomic<bool> started(false);

void sendStartedMessage(int clientSocket) {
    send(clientSocket, START_COMMAND.c_str(), START_COMMAND.length(), 0);
}
void handleClient(int clientSocket) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::lock_guard<std::mutex> lock(clientsMutex);
            // Erase the client socket from the vector
            clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
            closesocket(clientSocket);
            return;
        }

        buffer[bytesReceived] = '\0';
        std::string message(buffer);

        if (!started) {
            if (message == START_COMMAND) {
                std::cout << "Server started. Notifying client." << std::endl;
                started = true;
                sendStartedMessage(clientSocket);
            }
        } else {
            // Handle other messages or commands here
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error: WSAStartup failed." << std::endl;
        return 1;
    }

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error: Unable to create server socket. Error code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Unable to bind to port " << PORT << std::endl;
        closesocket(serverSocket);
        return 1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        std::cerr << "Error: Unable to listen on port " << PORT << std::endl;
        closesocket(serverSocket);
        return 1;
    }

    std::cout << "Waiting for clients to connect..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr); // Change to int from socklen_t
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error: Unable to accept connection." << std::endl;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            if (clients.size() >= MAX_CLIENTS) {
                std::cout << "Maximum clients reached. Closing connection." << std::endl;
                closesocket(clientSocket);
                continue;
            }
            clients.push_back(clientSocket);
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();

        if (started) {
            // Notify client that server has already started
            sendStartedMessage(clientSocket);
        }
    }

    closesocket(serverSocket);
    WSACleanup(); // Add this at the end of your main function

    return 0;
}
