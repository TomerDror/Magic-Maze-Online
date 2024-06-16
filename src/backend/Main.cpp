#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <algorithm>
#include <queue>
#include "Character.h"
#include "PreFieldPiece.h"
#include "FieldPiece.h"
#include "Player.h"
#include "Tile.h"

#include <windows.h>
#include <iostream>
#include <cstring>
#include <chrono>

bool stringToBool(const std::string &str);

void printPossibleMoves(MovementAbility *movementAbility);
void printPossibleMoves(Character *character, MovementAbility *movementAbility);
std::string movementAbilityToCmdStr(MovementAbility *movementAbility);
std::queue<std::string> splitString(const char input[]);
MovementAbility *queueToMovementAbility(std::queue<std::string> *cmd, MovementAbility *movementAbility);
void handleCmd(std::queue<std::string> *cmd);
bool canOpenFieldPiece(Character *character);
void openFieldPiece(Character *character);
std::vector<std::string> cmds;
std::string SERVER_IP = "127.0.0.1"; // Change this to the server's IP address
const int SERVER_PORT = 27015;
Player firstPlayer(0, 0, 0, 0, 0, 0, 0);
bool started = false;
bool printToText = true;
const int SHM_SIZE = 514; // 2 buffers of 256 bytes each + 2 flags
const int BUFFER_SIZE = 256;
bool tryToConnect = false;

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
        handleCmd(&cmd);
    }
}

HANDLE hMapFile;
char *pBuf;

void send_to_python(const std::string &message)
{
    if (message.length() > BUFFER_SIZE)
    {
        throw std::runtime_error("Message too long");
    }
    pBuf[1] = 1; // Indicate message is ready
    std::memcpy(pBuf + 258, message.c_str(), message.length());
    std::memset(pBuf + 258 + message.length(), 0, BUFFER_SIZE - message.length()); // Clear remaining bytes
}

std::string recv_from_python()
{
    if (pBuf[0] == 1)
    {
        std::string message(pBuf + 2, BUFFER_SIZE);
        auto null_char = std::find(message.begin(), message.end(), '\0');
        message.erase(null_char, message.end());
        pBuf[0] = 0; // Clear the flag
        return message;
    }
    return "";
}

void ClientMain()
{
    bool connected = false;
    SOCKET clientSocket;
    while (!connected)
    {
        while (!tryToConnect)
        {
        }
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            std::cerr << "WSAStartup failed: " << result << std::endl;
        }

         clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Error creating client socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP.c_str());
        serverAddr.sin_port = htons(SERVER_PORT);
        connected = true;
        result = connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr));
        if (result == SOCKET_ERROR)
        {
            connected = false;
            tryToConnect = false;
            std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            send_to_python("failed_login$"+SERVER_IP);
        }
    }
    std::cout << "Connected to server.";
    send_to_python("connected");
    std::thread serverThread(HandleServerMessages, clientSocket);

    // Main loop to send commands to the server
    while (true)
    {
        std::string cmdStr;

        if (printToText)
        {
            // std::cout<<" ";
            if (!cmds.empty())
            {
                cmdStr = cmds.front();
                cmds.pop_back();
                // if(started){
                //     std::cout<<"answer"
                // }
                // std::cout << "wed" << cmdStr << " " << started << "\n";
            }
        }
        else
        {
            std::getline(std::cin, cmdStr);
        }

        std::replace(cmdStr.begin(), cmdStr.end(), ' ', '$');
        std::queue<std::string> cmd = splitString(cmdStr.c_str());
        bool sendToOthers = false;
        if (started && cmd.front() == "getCharacter")
        {
            cmd.pop();
            if (cmd.front() == "green")
            {
                printPossibleMoves(Field::getInstance()->getGreenCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "purple")
            {
                printPossibleMoves(Field::getInstance()->getPurpleCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "orange")
            {
                printPossibleMoves(Field::getInstance()->getOrangeCharacter(), firstPlayer.movementAbility);
            }
            if (cmd.front() == "yellow")
            {
                printPossibleMoves(Field::getInstance()->getYellowCharacter(), firstPlayer.movementAbility);
            }
        }
        else if (started && cmd.front() == "get")
        {
            cmd.pop();

            if (std::stoi(cmd.front()) == Field::getInstance()->getGreenCharacter()->tileOn->tileType / 1000000)
            {
                std::cout << "green character\n";
                printPossibleMoves(Field::getInstance()->getGreenCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance()->getPurpleCharacter()->tileOn->tileType / 1000000)
            {
                std::cout << "purple character\n";
                printPossibleMoves(Field::getInstance()->getPurpleCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance()->getOrangeCharacter()->tileOn->tileType / 1000000)
            {
                std::cout << "orange character\n";
                printPossibleMoves(Field::getInstance()->getOrangeCharacter(), firstPlayer.movementAbility);
            }
            else if (std::stoi(cmd.front()) == Field::getInstance()->getYellowCharacter()->tileOn->tileType / 1000000)
            {
                std::cout << "yellow character\n";
                printPossibleMoves(Field::getInstance()->getYellowCharacter(), firstPlayer.movementAbility);
            }
            else
            {
                std::cout << "no character on square " << cmd.front();
                // printPossibleMoves(Field::getInstance()->getYellowCharacter(), firstPlayer.movementAbility);
            }
            std::cout << "\n";
        }
        else if (cmd.front() == "quit")
        {
            break;
        }
        else if (!started && cmd.front() == "start")
        {
            std::cout << "starting";
            sendToOthers = true;
        }
        else if (started && (cmd.front() == "move" || cmd.front() == "open"))
        {
            cmdStr.append(movementAbilityToCmdStr(firstPlayer.movementAbility));
            sendToOthers = true;
        }

        if (sendToOthers && send(clientSocket, cmdStr.c_str(), cmdStr.length(), 0) == SOCKET_ERROR)
        {
            std::cerr << "Error sending message to server: " << WSAGetLastError() << std::endl;
            break;
        }
    }
    closesocket(clientSocket);
    serverThread.join();

    WSACleanup();
}
void listen_for_python_messages()
{
    while (true)
    {
        std::string message = recv_from_python();
        if (!message.empty())
        {
            std::string prefix = "setIp";
            if (message.compare(0, prefix.length(), prefix) == 0)
            {
                std::queue<std::string> cmd = splitString(message.c_str());
                cmd.pop();
                SERVER_IP = cmd.front().c_str();
                tryToConnect = true;
            }
            else
            {
                std::cout << "Received from Python: " << message << std::endl;
                cmds.push_back(message);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

bool run_python_script(const char *script_name)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Construct the command line to run the Python script
    std::string command = "python ";
    command += script_name;

    // Start the Python script process
    if (!CreateProcessA(NULL, const_cast<char *>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return false;
    }

    // Close process and thread handles (Python script will continue to run independently)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

int main()
{
    // std::string ip;

    // std::cout << "Enter ip address of server: ";
    // std::getline(std::cin, ip);

    // std::cout << "You entered: " << ip << std::endl;
    // SERVER_IP =ip.c_str();
    while (!run_python_script("../../src/frontend/frontend.py"))
    {
        return 1;
    }
    // std::cout<<"a";
    for (int attempts = 0; attempts < 10; ++attempts)
    {
        hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "my_shared_memory");
        if (hMapFile != NULL)
        {
            break;
        }
        std::cerr << "Could not open file mapping object (" << GetLastError() << "), retrying..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (hMapFile == NULL)
    {
        std::cerr << "Could not open file mapping object after several attempts." << std::endl;
        return 1;
    }

    pBuf = (char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
    if (pBuf == NULL)
    {
        std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    std::thread listener_thread(listen_for_python_messages);
    listener_thread.detach();

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
    printPossibleMoves(Field::getInstance()->getGreenCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance()->getPurpleCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance()->getYellowCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance()->getOrangeCharacter(), movementAbility);
    // printPossibleMoves(getInstance().getGreenCharacter(),movementAbility);
}

void printPossibleMoves(Character *character, MovementAbility *movementAbility)
{
    std::cout << "possible " << (character->name) << " tiles ";
    std::string str;
    std::vector<Tile *> possibleTiles = character->getPlausibleTargetTiles(movementAbility);
    if (printToText)
        // outFile << character->name << "$";
        str = character->name;
    str.append("$");
    // Check if the file opened successfully

    for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)

    {

        {
            // Write data to the file
            str.append(std::to_string(possibleTiles[i]->tileType / 1000000));

            str.append("$");
        }
        // std::cout << possibleTiles[i]->tileType  << " ";
        std::cout << possibleTiles[i]->tileType / 1000000 << " ";
    }
    if (canOpenFieldPiece(character))
    {
        str.append(Utils::getDirection(character->tileOn->tileType));
    }
    else
    {
        str.append("none");
    }
    send_to_python(str);

}

void handleCmd(std::queue<std::string> *cmd)
{

    std::string color;
    int number = 0;
    MovementAbility *movementAbility = firstPlayer.movementAbility;
    if (cmd->front() == "quit")
    {
        exit(0);
    }
    // std::cout << "hey there " << cmd->front() << "\n";
    if (cmd->front() == "start")
    {

        cmd->pop();
        firstPlayer.movementAbility = queueToMovementAbility(cmd, firstPlayer.movementAbility);
        std::vector<int> myLinkedList;
        //
        // std::cout<<"k";
        while (!cmd->empty())
        {

            // for (int i = 0; i < stoi(cmd->front()); i++)

            myLinkedList.push_back(std::stoi(cmd->front()));
            // std::cout<<"\n here here "<<cmd->front();
            cmd->pop();
            // cmdStr.append("");
        }
        // std::cout<<"k";
        Field::getInstance()->futureFieldPieces = Utils::createQueueFromVector(myLinkedList);
        // send_to_python("start" +movementAbilityToCmdStr(firstPlayer.movementAbility));
        send_to_python("start" + movementAbilityToCmdStr(firstPlayer.movementAbility));
        started = true;
    }
    if (started && cmd->front() == "open")
    {
        cmd->pop();
        color = cmd->front();
        cmd->pop();
        Character *character;
        if (color == "green")
        {
            character = Field::getInstance()->getGreenCharacter();
        }
        if (color == "purple")
        {
            character = Field::getInstance()->getPurpleCharacter();
        }
        if (color == "orange")
        {
            character = Field::getInstance()->getOrangeCharacter();
        }
        if (color == "yellow")
        {
            character = Field::getInstance()->getYellowCharacter();
        }
        std::cout << "request to open";
        if (canOpenFieldPiece(character))
        {
            std::cout << "able to open";
            openFieldPiece(character);
            std::cout<<"open";
            double fpInDir = Utils::getDirection(character->tileOn->tileType) == "up" ? (character->tileOn->tileAbove->tileType) : Utils::getDirection(character->tileOn->tileType) == "down" ? (character->tileOn->tileBellow->tileType)
                                                                                                                               : Utils::getDirection(character->tileOn->tileType) == "left"   ? (character->tileOn->tileToLeft->tileType)
                                                                                                                                                                                              : (character->tileOn->tileToRight->tileType - 1);
            std::cout<<"open2";
            send_to_python("open$" + color + "$" + std::to_string((character->tileOn->tileType - 1) / 16 + 1) + "$" + Utils::getDirection(character->tileOn->tileType) + "$" + std::to_string(fpInDir));
            std::string a= "open$" + color + "$" + std::to_string((character->tileOn->tileType - 1) / 16 + 1) + "$" + Utils::getDirection(character->tileOn->tileType) + "$" + std::to_string(fpInDir);
            std::cout<<"hellow"<<a;
        }
    }
    if (started && cmd->front() == "move")
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
            std::vector<Tile *> possibleTiles = Field::getInstance()->getGreenCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {

                if (possibleTiles[i]->tileType / 1000000 == number)
                {
                    // std::cout << " value is is: " << possibleTiles[i]->tileType / 1000000;
                    didMove = true;

                    Field::getInstance()->getGreenCharacter()->move(possibleTiles[i], movementAbility);
                    // std::ofstream outFile("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt");
                    // if (outFile.is_open())
                    // outFile << "move$" << color << "$" << number;
                    send_to_python("move$" + color + "$" + std::to_string(number));
                }
            }
        }
        else if (color == "purple")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance()->getPurpleCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 1000000 == number)
                {
                    didMove = true;

                    Field::getInstance()->getPurpleCharacter()->move(possibleTiles[i], movementAbility);
                    send_to_python("move$" + color + "$" + std::to_string(number));
                }
            }
        }
        else if (color == "orange")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance()->getOrangeCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 1000000 == number)
                {
                    didMove = true;

                    Field::getInstance()->getOrangeCharacter()->move(possibleTiles[i], movementAbility);
                    send_to_python("move$" + color + "$" + std::to_string(number));
                }
            }
        }
        else if (color == "yellow")
        {
            std::vector<Tile *> possibleTiles = Field::getInstance()->getYellowCharacter()->getPlausibleTargetTiles(movementAbility);
            for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
            {
                if (possibleTiles[i]->tileType / 1000000 == number)
                {
                    didMove = true;

                    Field::getInstance()->getYellowCharacter()->move(possibleTiles[i], movementAbility);
                    send_to_python("move$" + color + "$" + std::to_string(number));
                }
            }
        }

        if (!didMove)
        {

            send_to_python("move$nowhere");
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
bool canOpenFieldPiece(Character *character)
{
    // std::cout<<"please";
    // std::cout<<"\n";
    // std::cout<<"is opening "<< (Utils::getTileFeature(character->tileOn->tileType) )<<"  tile color " << Utils::getTileColor(character->tileOn->tileType) << " character color "<< character->name;
    bool canOpen = Utils::getTileFeature(character->tileOn->tileType) == "opening" && Utils::getTileColor(character->tileOn->tileType) == character->name;
    if (Utils::getDirection(character->tileOn->tileType) == "up")
    {
        canOpen = canOpen && character->tileOn->tileAbove == nullptr;
    }
    if (Utils::getDirection(character->tileOn->tileType) == "down")
    {
        canOpen = canOpen && character->tileOn->tileBellow == nullptr;
    }
    if (Utils::getDirection(character->tileOn->tileType) == "right")
    {
        canOpen = canOpen && character->tileOn->tileToRight == nullptr;
    }
    if (Utils::getDirection(character->tileOn->tileType) == "left")
    {
        // std::cout<< canOpen<<" ";
        canOpen = canOpen && character->tileOn->tileToLeft == nullptr;
    }
    // std::cout << "can open" << canOpen << "\n";
    return canOpen;
}

void openFieldPiece(Character *character)
{
    character->openFieldPiece();
    // FieldPiece newFieldPiece = FieldPiece(Field::getInstance().allFieldPieces[Field::getInstance().futureFieldPieces]);
}
