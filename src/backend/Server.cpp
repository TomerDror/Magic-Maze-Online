    #include <iostream>
    #include <WinSock2.h>
    #include <vector>
    #include <thread>


    const int DEFAULT_PORT = 27015;
    const int MAX_CLIENTS = 10;
    const int BUFFER_SIZE = 1024;

    std::vector<SOCKET> clients;
    SOCKET serverSocket;
    bool gameStarted = false;

    void BroadcastMessage(const char* message, int size) {
        for (SOCKET client : clients) {
            send(client, message, size, 0);
        }
    }

    void ClientHandler(SOCKET clientSocket) {
        char buffer[BUFFER_SIZE];
        int bytesReceived;

        while (true) {
            bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                std::cerr << "Error receiving from client. Closing connection." << std::endl;
                closesocket(clientSocket);
                return;
            }

            buffer[bytesReceived] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;

            if (!gameStarted && strcmp(buffer, "start") == 0) {
                std::cout << "Start command received. Broadcasting start to all clients." << std::endl;
                gameStarted = true;
                BroadcastMessage("start", strlen("start"));
            } else if (gameStarted) {
                // Broadcast received message to all clients
                BroadcastMessage(buffer, bytesReceived);
            }
        }
    }

    int main() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            return 1;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating server socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(DEFAULT_PORT);

        result = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
        if (result == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        result = listen(serverSocket, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Server started. Waiting for clients..." << std::endl;

        while (true) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                closesocket(serverSocket);
                WSACleanup();
                return 1;
            }

            if (clients.size() >= MAX_CLIENTS) {
                std::cerr << "Maximum clients reached. Closing new connection." << std::endl;
                closesocket(clientSocket);
            } else {
                clients.push_back(clientSocket);
                std::cout << "Client connected. Total clients: " << clients.size() << std::endl;
                std::thread clientThread(ClientHandler, clientSocket);
                clientThread.detach(); // Detach thread so it can run independently
            }
        }

        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
