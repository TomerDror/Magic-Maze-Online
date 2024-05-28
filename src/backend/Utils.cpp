#include "Utils.h"
std::string Utils::getTileFeature(int tileValue)
{
    int feature = extractBits(tileValue % 1000000, Constants::tileTypeFeatureStart, Constants::tileTypeFeatureEnd);
    switch (feature)
    {
    case 0:
        return "inaccessable";
        break;
    case 1:
        return "empty";
        break;
    case 2:
        return "exit";
        break;
    case 3:
        return "store";
        break;
    case 4:
        return "opening";
        break;
    case 5:
        return "portal";
        break;
    case 6:
        return "escalator";
        break;
    case 7:
        return "time-flip";
        break;
    case 8:
        return "entrance";
        break;
    default:
        return "function error";
        break;
    }
}
std::string Utils::getTileColor(int tileValue)
{
    int color = extractBits(tileValue % 1000000, Constants::tileTypeColorStart, Constants::tileTypeColorEnd);
    if (color == 0)
    {
        return "green";
    }
    if (color == 1)
    {
        return "purple";
    }
    if (color == 2)
    {
        return "orange";
    }
    if (color == 3)
    {
        return "yellow";
    }
    return "no-color";
}

int Utils::getDirectionBitwise(int tileValue)
{
    return extractBits(tileValue % 1000000, Constants::tileTypeDirectionStart, Constants::tileTypeDirectionEnd);
}
std::string Utils::getDirection(int tileValue)
{
    int bitwiseDirection = Utils::getDirectionBitwise(tileValue);
    switch (bitwiseDirection)
    {
    case 0000:
        return "none";
        break;
    case 1: // 0001
        return "right";
    case 2: // 0010
        return "left";
    case 4: // 0100
        return "down";
    case 5: // 0101
        return "down-right";
    case 6: // 0110
        return "down-left";
    case 8: // 1000
        return "up";
    case 9: // 1001
        return "up-right";
    case 10: // 1010
        return "up-left";

    default:
        return "invalid direction";
        break;
    }
}

void Utils::setDirection(int *tileValue, int directionBitwise)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, directionBitwise, Constants::tileTypeDirectionStart, Constants::tileTypeDirectionEnd);
}
void Utils::setTileFeature(int *tileValue, int feature)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, feature, Constants::tileTypeFeatureStart, Constants::tileTypeFeatureEnd);
}

int Utils::getEscalatorDirectionBitwise(int tileValue)
{
    return extractBits(tileValue % 1000000, Constants::tileTypeEscalatorDirectionStart, Constants::tileTypeEscalatorDirectionEnd);
}
std::string Utils::getEscalatorDirection(int tileValue)
{
    int bitwiseDirection = Utils::getEscalatorDirectionBitwise(tileValue);
    switch (bitwiseDirection)
    {
    case 0000:
        return "none";
        break;
    case 1: // 0001
        return "clock 1";
    case 2: // 0010
        return "clock 2";
    case 4: // 0100
        return "clock 4";
    case 5: // 0101
        return "clock 5";
    case 7: // 0110
        return "clock 7";
    case 8: // 1000
        return "clock 8";
    case 10: // 1001
        return "clock 10";
    case 11: // 1010
        return "clock 11";
    case 12:
        return "up-right";
    case 13:
        return "down-right";
    case 14:
        return "down-left";
    case 15:
        return "up-left";

    default:
        return "invalid direction";
        break;
    }
}

void Utils::setEscalatorDirection(int *tileValue, int directionBitwise)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, directionBitwise, Constants::tileTypeEscalatorDirectionStart, Constants::tileTypeEscalatorDirectionEnd);
}

void Utils::rotateDirectionLeft(int *tileValue)
{
    int bitwise = Utils::getDirectionBitwise(*tileValue);
    int right = bitwise % 2;

    bitwise = Utils::setBits(bitwise, (bitwise / 4) % 2, 1, 1); // set right from down
    bitwise = Utils::setBits(bitwise, (bitwise / 2) % 2, 3, 3); // set down from left
    bitwise = Utils::setBits(bitwise, (bitwise / 8) % 2, 2, 2); // set left from up
    bitwise = Utils::setBits(bitwise, right, 4, 4);             // set up from right
    Utils::setDirection(tileValue, bitwise);

    bitwise = Utils::getEscalatorDirectionBitwise(*tileValue);
    // clock rotation
    if (bitwise > 0 && bitwise <= 11)
    {
        Utils::setEscalatorDirection(tileValue, (bitwise + 9) % 12);
    }
    // corner rotation
    if (bitwise >= 12)
    {
        Utils::setEscalatorDirection(tileValue, (bitwise - 11) % 4 + 12);
    }

    int canMoveRight = Utils::tileBlockedMoveRight(*tileValue);
    // std::cout<<(*tileValue)<<"\n";
    // std::cout << " right " << Utils::tileBlockedMoveRight(*tileValue);
    Utils::setTileBlockedMoveRight(tileValue, Utils::tileBlockedMoveDown(*tileValue));
    // std::cout << "  " << Utils::tileBlockedMoveRight(*tileValue);

    // std::cout << " down " << Utils::tileBlockedMoveDown(*tileValue);
    Utils::setTileBlockedMoveDown(tileValue, Utils::tileBlockedMoveLeft(*tileValue));
    // std::cout << "  " << Utils::tileBlockedMoveDown(*tileValue);

    // std::cout << " left " << Utils::tileBlockedMoveLeft(*tileValue);
    Utils::setTileBlockedMoveLeft(tileValue, Utils::tileBlockedMoveUp(*tileValue));
    // std::cout << "  " << Utils::tileBlockedMoveLeft(*tileValue);

    // std::cout << " up " << Utils::tileBlockedMoveUp(*tileValue);
    Utils::setTileBlockedMoveUp(tileValue, canMoveRight);
    // std::cout << "  " << Utils::tileBlockedMoveUp(*tileValue)<<"\n";
    // std::cout<<(*tileValue)<<"\n";
    // std::cout<<"\n";
}

void Utils::setTileBlockedMoveUp(int *tileValue, bool blockedUp)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, !blockedUp, Constants::tileTypeBlockedMoveUp, Constants::tileTypeBlockedMoveUp);
}
bool Utils::tileBlockedMoveUp(int tileValue)
{
    return !Utils::extractBits(tileValue % 1000000, Constants::tileTypeBlockedMoveUp, Constants::tileTypeBlockedMoveUp);
}

void Utils::setTileBlockedMoveDown(int *tileValue, bool blockedDown)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, !blockedDown, Constants::tileTypeBlockedMoveDown, Constants::tileTypeBlockedMoveDown);
}
bool Utils::tileBlockedMoveDown(int tileValue)
{
    return !Utils::extractBits(tileValue % 1000000, Constants::tileTypeBlockedMoveDown, Constants::tileTypeBlockedMoveDown);
}

void Utils::setTileBlockedMoveLeft(int *tileValue, bool blockedLeft)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, !blockedLeft, Constants::tileTypeBlockedMoveLeft, Constants::tileTypeBlockedMoveLeft);
}
bool Utils::tileBlockedMoveLeft(int tileValue)
{
    return !Utils::extractBits(tileValue % 1000000, Constants::tileTypeBlockedMoveLeft, Constants::tileTypeBlockedMoveLeft);
}

void Utils::setTileBlockedMoveRight(int *tileValue, bool blockedRight)
{
    int temp = *tileValue / 1000000;
    *tileValue = temp * 1000000 + Utils::setBits(*tileValue % 1000000, !blockedRight, Constants::tileTypeBlockedMoveRight, Constants::tileTypeBlockedMoveRight);
}

bool Utils::tileBlockedMoveRight(int tileValue)
{
    return !Utils::extractBits(tileValue % 1000000, Constants::tileTypeBlockedMoveRight, Constants::tileTypeBlockedMoveRight);
}

int Utils::setBits(int num, int value, int start, int end)
{
    // Create a mask to set the specified bits to 1
    int mask = ((1 << (end - start + 1)) - 1) << (start - 1);

    // Clear the bits in num that we're going to set
    num &= ~mask;

    // Shift and mask the value to fit in the specified range
    value = (value << (start - 1)) & mask;

    // Set the bits in num to the masked value
    return num | value;
}

int Utils::extractBits(int num, int start, int end)
{
    // start -=1;
    int mask = ((1 << (end - start + 1)) - 1) << (start - 1);
    return (num & mask) >> (start - 1);
}

std::queue<int> Utils::createQueueFromVector(const std::vector<int> &vec)
{
    std::queue<int> q;
    for (const auto &element : vec)
    {
        q.push(element);
    }
    return q;
}

int **Utils::readDatabaseFromFile(const std::string &filePath, int targetID)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file" << std::endl;
        return nullptr;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream headerStream(line);
        int id, width, length;
        headerStream >> id >> width >> length;

        if (id == targetID)
        {
            int **matrix = new int *[length];
            for (int i = 0; i < length; ++i)
            {
                matrix[i] = new int[width];
            }

            for (int i = 0; i < length; ++i)
            {
                if (!std::getline(file, line))
                {
                    std::cerr << "Error reading matrix data" << std::endl;
                    return nullptr;
                }
                std::istringstream rowStream(line);
                for (int j = 0; j < width; ++j)
                {
                    // std::cout<<matrix[i][j];
                    rowStream >> matrix[i][j];
                }
            }
            file.close();
            return matrix;
        }
        else
        {
            // Skip the matrix lines
            for (int i = 0; i < length; ++i)
            {
                std::getline(file, line);
            }
        }
    }

    file.close();
    std::cerr << "ID not found" << std::endl;
    return nullptr;
}
