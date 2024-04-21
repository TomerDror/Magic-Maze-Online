#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <algorithm>
#include "Character.h"
#include "PreFieldPiece.h"
#include "FieldPiece.h"
#include "Player.h"
#include "Tile.h"
#include <iostream>
#include <fstream>

#include <queue>

bool stringToBool(const std::string &str);

void printPossibleMoves(MovementAbility *movementAbility);
void printPossibleMoves(Character *character, MovementAbility *movementAbility);
std::string movementAbilityToCmdStr(MovementAbility *movementAbility);
std::queue<std::string> splitString(const char input[]);
MovementAbility *queueToMovementAbility(std::queue<std::string> *cmd, MovementAbility *movementAbility);
void handleCmd(std::queue<std::string> *cmd);

const char *SERVER_IP = "127.0.0.1"; // Change this to the server's IP address
const int SERVER_PORT = 27015;
const int BUFFER_SIZE = 1024;
Player firstPlayer(1, 0, 1, 1, 1, 1, 1);

bool printToText = false;
void HandleServerMessages(SOCKET serverSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    // printPossibleMoves(firstPlayer.movementAbility);
    while (true)
    {
        bytesReceived = recv(serverSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
        {
            std::cerr << "Error receiving from server. Connection closed." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received from server: " << buffer << std::endl;
        std::queue<std::string> cmd = splitString(buffer);
        handleCmd(&cmd);
        // printPossibleMoves(firstPlayer.movementAbility);
    }
}

void ClientMain()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error creating client socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    result = connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to server.";
    std::thread serverThread(HandleServerMessages, clientSocket);

    // // Start a thread to handle server messages
    // if (exitCode == 0)
    // {
    //     std::cout << "Python script executed successfully." << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Error: Python script execution failed." << std::endl;
    //     return ; // Return a non-zero value to indicate failure
    // }
    // Main loop to send commands to the server
    while (true)
    {
        std::string cmdStr;
        std::string color;
        // int number = 0;

        // std::cout << "Enter a command: ";
        // std::getline(std::cin, cmdStr);
        if (printToText)
        {
            std::ifstream MyReadFile("C://Users//tomer//Documents//school//cpProject//src//extras//toCpp.txt");

            if (MyReadFile.is_open())
            {                                // Check if file is open
                getline(MyReadFile, cmdStr); // Read a line from the file into cmdStr
                if (cmdStr.empty())
                {
                    MyReadFile.close(); // Close the file
                }
                else
                {

                    std::cout << cmdStr;
                    MyReadFile.close(); // Close the file

                    std::ofstream file("C://Users//tomer//Documents//school//cpProject//src//extras//toCpp.txt", std::ofstream::out | std::ofstream::trunc);

                    if (printToText && file.is_open())
                    {
                        file.close();
                        //     // std::cout << "File contents deleted successfully." << std::endl;
                    }
                    // else
                    // {
                    //     std::cerr << "Unable to open file!";
                    //     return;
                    // }
                }
            }
        }
        else
        {
            std::cout << "Enter a command: ";
            std::getline(std::cin, cmdStr);
        }

        std::replace(cmdStr.begin(), cmdStr.end(), ' ', '$');
        std::queue<std::string> cmd = splitString(cmdStr.c_str());
        bool sendToOthers = false;
        if (cmd.front() == "getCharacter")
        {
            std::cout << "klhsd";
            cmd.pop();
            if (cmd.front() == "green")
            {
                printPossibleMoves(Field::getInstance().getGreenCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "purple")
            {
                printPossibleMoves(Field::getInstance().getPurpleCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "orange")
            {
                printPossibleMoves(Field::getInstance().getOrangeCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "yellow")
            {
                printPossibleMoves(Field::getInstance().getYellowCharacter(), firstPlayer.movementAbility);
            }
        }
        else if (cmd.front() == "get")
        {
            cmd.pop();
            if (std::stoi(cmd.front()) == Field::getInstance().getGreenCharacter()->tileOn->tileType / 100000)
            {
                std::cout << "green character\n";
                printPossibleMoves(Field::getInstance().getGreenCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance().getPurpleCharacter()->tileOn->tileType / 100000)
            {
                std::cout << "purple character\n";
                printPossibleMoves(Field::getInstance().getPurpleCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance().getOrangeCharacter()->tileOn->tileType / 100000)
            {
                std::cout << "orange character\n";
                printPossibleMoves(Field::getInstance().getOrangeCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance().getYellowCharacter()->tileOn->tileType / 100000)
            {
                std::cout << "yellow character\n";
                printPossibleMoves(Field::getInstance().getYellowCharacter(), firstPlayer.movementAbility);
            }
            else
            {
                std::cout << "no character on squere " << cmd.front();
                printPossibleMoves(Field::getInstance().getYellowCharacter(), firstPlayer.movementAbility);
            }
            std::cout << "\n";
        }

        else if (cmd.front() == "quit")
        {
            break;
        }
        else if (cmd.front() == "move")
        {
            // std::cout << "moving";
            cmdStr.append(movementAbilityToCmdStr(firstPlayer.movementAbility));
            sendToOthers = true;
        }

        if (sendToOthers && send(clientSocket, cmdStr.c_str(), cmdStr.length(), 0) == SOCKET_ERROR)
        {
            std::cerr << "Error sending message to server: " << WSAGetLastError() << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Sleep for 100 milliseconds
        // Close the socket and join the server thread
    }
        closesocket(clientSocket);
        serverThread.join();

        WSACleanup();
}

int main()
{
    // int a = 1206240;
    // Utils::rotateDiractionLeft(&a);

    // std::cout<<"\n the value is: "<< a;

    std::thread clientThread(ClientMain);
    clientThread.join();

    return 0;
}

std::string movementAbilityToCmdStr(MovementAbility *movementAbility)
{
    return "$" + std::to_string(movementAbility->canMoveUp) +
           "$" + std::to_string(movementAbility->canMoveDown) +
           "$" + std::to_string(movementAbility->canMoveLeft) +
           "$" + std::to_string(movementAbility->canMoveRight) +
           "$" + std::to_string(movementAbility->canUseEscalator) +
           "$" + std::to_string(movementAbility->canUsePortals) +
           "$" + std::to_string(movementAbility->canOpenFieldPiece);
}

std::queue<std::string> splitString(const char input[])
{
    std::queue<std::string> result;
    size_t len = strlen(input);
    size_t start = 0;

    for (size_t i = 0; i <= len; ++i)
    {
        if (input[i] == '$' || input[i] == '\0')
        {
            result.push(std::string(input + start, i - start));
            start = i + 1;
        }
    }

    return result;
}

void printPossibleMoves(MovementAbility *movementAbility)
{
    printPossibleMoves(Field::getInstance().getGreenCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getPurpleCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getYellowCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getOrangeCharacter(), movementAbility);
    // printPossibleMoves(getInstance().getGreenCharacter(),movementAbility);
}

void printPossibleMoves(Character *character, MovementAbility *movementAbility)
{
    std::cout << "adsf";
    std::cout << "possible " << (character->name) << " tiles ";
    std::vector<Tile *> possibleTiles = character->getPlausibleTargetTiles(movementAbility);
    std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
    if (printToText && outFile.is_open())
        outFile << character->name << "$";
    // Check if the file opened successfully

    for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)

    {

        if (printToText && outFile.is_open())
        {
            // Write data to the file
            outFile << possibleTiles[i]->tileType / 100000;
            if (i != possibleTiles.size() - 1)
            {
                outFile << "$";
            }
        }
        std::cout << possibleTiles[i]->tileType / 100000 << " ";
    }
    if (printToText && outFile.is_open())
        outFile.close();

    std::cout << "\n";
}

void handleCmd(std::queue<std::string> *cmd)
{

    std::string color;
    int number = 0;
    MovementAbility *movementAbility = new MovementAbility();
    if (cmd->front() == "stop")
    {
        exit(0);
    }
    if (cmd->front() == "move")
    {
        cmd->pop();

        color = cmd->front();
        cmd->pop();
        number = std::stoi(cmd->front());
        cmd->pop();
        // std::cout<<color<<" \n";
        bool didMove = false;
        movementAbility = queueToMovementAbility(cmd, movementAbility);
        if (color == "green")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getGreenCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 100000 == number)
                {
                    didMove = true;

                    Field::getInstance().getGreenCharacter()->move(possibleTiles[i], movementAbility);
                    std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
                    if (outFile.is_open())
                        outFile << "move$" << color << "$" << number;
                }
            }
        }
        else if (color == "purple")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getPurpleCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 100000 == number)
                {
                    didMove = true;

                    Field::getInstance().getPurpleCharacter()->move(possibleTiles[i], movementAbility);
                    std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
                    if (outFile.is_open())
                        outFile << "move$" << color << "$" << number;
                }
            }
        }
        else if (color == "orange")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getOrangeCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 100000 == number)
                {
                    didMove = true;

                    Field::getInstance().getOrangeCharacter()->move(possibleTiles[i], movementAbility);
                    std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
                    if (outFile.is_open())
                        outFile << "move$" << color << "$" << number;
                }
            }
        }
        else if (color == "yellow")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getYellowCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 100000 == number)
                {
                    didMove = true;

                    std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
                    if (outFile.is_open())
                        outFile << "move$" << color << "$" << number;
                    Field::getInstance().getYellowCharacter()->move(possibleTiles[i], movementAbility);
                }
            }
        }

        if (!didMove)
        {
            std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
            if (outFile.is_open())
                outFile << "move$nowhere";
            std::cout << "moving nowhere";
        }
    }
}

MovementAbility *queueToMovementAbility(std::queue<std::string> *cmd, MovementAbility *movementAbility)
{
    movementAbility->canMoveUp = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canMoveDown = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canMoveLeft = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canMoveRight = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canUseEscalator = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canUsePortals = stringToBool(cmd->front());
    cmd->pop();
    movementAbility->canOpenFieldPiece = stringToBool(cmd->front());
    cmd->pop();
    return movementAbility;
}

bool stringToBool(const std::string &str)
{
    // Convert the string to lowercase for case-insensitive comparison
    std::string lowerStr;
    for (char c : str)
    {
        lowerStr += std::tolower(c);
    }

    // Check if the string represents true
    if (lowerStr == "true" || lowerStr == "yes" || lowerStr == "1")
    {
        return true;
    }
    // Check if the string represents false
    else if (lowerStr == "false" || lowerStr == "no" || lowerStr == "0")
    {
        return false;
    }
    // Invalid string representation
    else
    {
        // Handle error, throw exception, or return a default value
        throw std::invalid_argument("Invalid boolean string representation: " + str);
    }
}
