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
#include <queue>

bool stringToBool(const std::string &str);

void printPossibleMoves(MovementAbility *movementAbility);
void printPossibleMoves(Character *character, MovementAbility *movementAbility);
std::string movementAbilityToCmdStr(MovementAbility *movementAbility);
std::queue<std::string> splitString(const char input[]);
MovementAbility *queueToMovementAbility(std::queue<std::string> *cmd,MovementAbility *movementAbility);
void handleCmd(std::queue<std::string> *cmd);

const char *SERVER_IP = "127.0.0.1"; // Change this to the server's IP address
const int SERVER_PORT = 27015;
const int BUFFER_SIZE = 1024;
Player firstPlayer(1, 1, 0, 1, 0, 0, 0);

void HandleServerMessages(SOCKET serverSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived;

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

        printPossibleMoves(firstPlayer.movementAbility);

        handleCmd(&cmd);
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

    std::cout << "Connected to server." << std::endl;

    // Start a thread to handle server messages
    std::thread serverThread(HandleServerMessages, clientSocket);

    // Main loop to send commands to the server
    while (true)
    {
        std::string cmd;
        std::string color;
        // int number = 0;

        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        std::replace(cmd.begin(), cmd.end(), ' ', '$');
        if (cmd != "start")
            cmd.append(movementAbilityToCmdStr(firstPlayer.movementAbility));

        if (send(clientSocket, cmd.c_str(), cmd.length(), 0) == SOCKET_ERROR)
        {
            std::cerr << "Error sending message to server: " << WSAGetLastError() << std::endl;
            break;
        }

        if (cmd == "quit")
        {
            break;
        }
    }

    // Close the socket and join the server thread
    closesocket(clientSocket);
    serverThread.join();

    WSACleanup();
}

int main()
{

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
    std::cout << "possible " << (character->name) << " tiles ";
    std::vector<Tile *> possibleTiles = character->getPlausibleTargetTiles(movementAbility);
    for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
    {

        std::cout << possibleTiles[i]->tileType << " ";
    }
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
        std::cout<<color<<" \n";

            movementAbility = queueToMovementAbility(cmd, movementAbility);
        if (color == "green")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getGreenCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                std::cout<< possibleTiles[i]->tileType;
                std::cout<< number;
                std::cout<< "\n";
                std::cout<< movementAbility->canMoveUp;
                std::cout<< movementAbility->canMoveDown;
                std::cout<< movementAbility->canMoveLeft;
                std::cout<< movementAbility->canMoveRight;
                std::cout<< movementAbility->canUseEscalator;
                std::cout<< "\n";
                if (possibleTiles[i]->tileType == number)
                {
                    Field::getInstance().getGreenCharacter()->move(possibleTiles[i], movementAbility);
                }
            }
        }
        if (color == "purple")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getPurpleCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType == number)
                {
                    Field::getInstance().getPurpleCharacter()->move(possibleTiles[i], movementAbility);
                }
            }
        }
        if (color == "orange")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getOrangeCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType == number)
                {
                    Field::getInstance().getOrangeCharacter()->move(possibleTiles[i], movementAbility);
                }
            }
        }
        if (color == "yellow")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance().getYellowCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType == number)
                {

                    Field::getInstance().getYellowCharacter()->move(possibleTiles[i], movementAbility);
                }
            }
        }
    }
}

MovementAbility *queueToMovementAbility(std::queue<std::string> *cmd,MovementAbility *movementAbility)
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

// int main() {
// Player firstPlayer(1,1,0,1,0,0,0);

//     while(true){
//         std::string cmd;
//         std::string color;
//         int number = 0;

//         std::cout << "Enter a command: ";
//         std::cin >> cmd;
//         cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

//         if(cmd =="stop"){
//             break;
//         }
//         if (cmd == "move") {
//             std::cout << "Enter a color: ";
//             std::cin >> color;

//             while (true) {
//                 std::cout << "Enter a number: ";
//                 if (!(std::cin >> number)) {
//                     std::cout << "Please enter a valid number." << std::endl;
//                     std::cin.clear();
//                     std::cin.ignore(100, '\n');
//                 } else {
//                     break;
//                 }
//             }

//             color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
//             if(color == "green"){
//                 std::vector<Tile*> possibleTiles = Field::getInstance().getGreenCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
//                 for (std::vector<Tile*>::size_type i = 0; i < possibleTiles.size(); ++i) {
//                     if(possibleTiles[i]->tileType  == number){
//                         Field::getInstance().getGreenCharacter()->move(possibleTiles[i],firstPlayer.movementAbility);
//                     }
//                 }
//             }
//             if(color == "purple"){
//                 std::vector<Tile*> possibleTiles = Field::getInstance().getPurpleCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
//                 for (std::vector<Tile*>::size_type i = 0; i < possibleTiles.size(); ++i) {
//                     if(possibleTiles[i]->tileType  == number){
//                         Field::getInstance().getPurpleCharacter()->move(possibleTiles[i],firstPlayer.movementAbility);
//                     }
//                 }
//             }
//             if(color == "orange"){
//                 std::vector<Tile*> possibleTiles = Field::getInstance().getOrangeCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
//                 for (std::vector<Tile*>::size_type i = 0; i < possibleTiles.size(); ++i) {
//                     if(possibleTiles[i]->tileType  == number){
//                         Field::getInstance().getOrangeCharacter()->move(possibleTiles[i],firstPlayer.movementAbility);
//                     }
//                 }
//             }
//             if(color == "yellow"){
//                 std::vector<Tile*> possibleTiles = Field::getInstance().getYellowCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
//                 for (std::vector<Tile*>::size_type i = 0; i < possibleTiles.size(); ++i) {
//                     if(possibleTiles[i]->tileType  == number){
//                         Field::getInstance().getYellowCharacter()->move(possibleTiles[i],firstPlayer.movementAbility);
//                     }
//                 }
//             }
//         }
//     }//move green 102

//     return 0;
// }
