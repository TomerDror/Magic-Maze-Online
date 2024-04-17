#include "Utils.h"
std::string Utils::getTileFeature(int tileValue)
{
    int feature = extractBits(tileValue % 100000, Constants::tileTypeFeatureStart, Constants::tileTypeFeatureEnd);
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
    int color = extractBits(tileValue % 100000, Constants::tileTypeColorStart, Constants::tileTypeColorEnd);
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
    return extractBits(tileValue % 100000, Constants::tileTypeDirectionStart, Constants::tileTypeDirectionEnd);
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

void Utils::setDiraction(int *tileValue, int diractionBitwise)
{
    int temp = *tileValue / 100000;
    *tileValue = temp + Utils::setBits(*tileValue % 100000, diractionBitwise, Constants::tileTypeDirectionStart, Constants::tileTypeDirectionEnd);
}

void Utils::rotateDiractionLeft(int *tileValue)
{
    int bitwise = Utils::getDirectionBitwise(*tileValue);
    int right = bitwise % 2;
    bitwise = Utils::setBits(bitwise, (bitwise / 4) % 2, 1, 1); // set right from down
    bitwise = Utils::setBits(bitwise, (bitwise / 2) % 2, 3, 3); // set down from left
    bitwise = Utils::setBits(bitwise, (bitwise / 8) % 2, 2, 2); // set left from up
    bitwise = Utils::setBits(bitwise, right, 4, 4);     // set up from right
    Utils::setDiraction(tileValue,bitwise);


    int canMoveRight = Utils::tileBlockedMoveRight(*tileValue);
    Utils::setTileBlockedMoveRight(tileValue,Utils::tileBlockedMoveDown);
    Utils::setTileBlockedMoveDown(tileValue,Utils::tileBlockedMoveLeft);
    Utils::setTileBlockedMoveLeft(tileValue,Utils::tileBlockedMoveUp);
    Utils::setTileBlockedMoveUp(tileValue,canMoveRight);

}

void Utils::setTileBlockedMoveUp(int *tileValue, bool blockedUp)
{
    int temp = *tileValue / 100000;
    *tileValue = temp + Utils::setBits(*tileValue % 100000, blockedUp, Constants::tileTypeBlockedMoveUp, Constants::tileTypeBlockedMoveUp);
}
bool Utils::tileBlockedMoveUp(int tileValue)
{
    return !Utils::extractBits(tileValue % 100000, Constants::tileTypeBlockedMoveUp, Constants::tileTypeBlockedMoveUp);
}

void Utils::setTileBlockedMoveDown(int *tileValue, bool blockedDown)
{
    int temp = *tileValue / 100000;
    *tileValue = temp + Utils::setBits(*tileValue % 100000, blockedDown, Constants::tileTypeBlockedMoveDown, Constants::tileTypeBlockedMoveDown);
}
bool Utils::tileBlockedMoveDown(int tileValue)
{
    return !Utils::extractBits(tileValue % 100000, Constants::tileTypeBlockedMoveDown, Constants::tileTypeBlockedMoveDown);
}

void Utils::setTileBlockedMoveLeft(int *tileValue, bool blockedLeft)
{
    int temp = *tileValue / 100000;
    *tileValue = temp + Utils::setBits(*tileValue % 100000, blockedLeft, Constants::tileTypeBlockedMoveLeft, Constants::tileTypeBlockedMoveLeft);
}
bool Utils::tileBlockedMoveLeft(int tileValue)
{
    return !Utils::extractBits(tileValue % 100000, Constants::tileTypeBlockedMoveLeft, Constants::tileTypeBlockedMoveLeft);
}

void Utils::setTileBlockedMoveRight(int *tileValue, bool blockedRight)
{
    int temp = *tileValue / 100000;
    *tileValue = temp + Utils::setBits(*tileValue % 100000, blockedRight, Constants::tileTypeBlockedMoveRight, Constants::tileTypeBlockedMoveRight);
}

bool Utils::tileBlockedMoveRight(int tileValue)
{
    return !Utils::extractBits(tileValue % 100000, Constants::tileTypeBlockedMoveRight, Constants::tileTypeBlockedMoveRight);
}

int Utils::extractBits(int num, int start, int end)
{
    // start -=1;
    int mask = ((1 << (end - start + 1)) - 1) << (start - 1);
    return (num & mask) >> (start - 1);

    // int mask = (1 << (end - start + 1)) - 1;
    // mask <<= start;
    // return (num & mask) >> start;
}
int Utils::setBits(int num, int value, int start, int end)
{
    return num + value + start + end;
    // Todo
}