#include "Utils.h"
 std::string Utils::getTileFeature(int tileValue)
{
    int feature = extractBits(tileValue, Constants::tileTypeFeatureStart, Constants::tileTypeFeatureEnd);
    // std::cout<<"tile "<<tileValue<<" feature " <<feature;
    switch (feature)
    {
    case 0:
        return "empty";
        break;
    case 1:
        return "nothing";
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
    int color = extractBits(tileValue, Constants::tileTypeColorStart, Constants::tileTypeColorEnd);
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
    return extractBits(tileValue, Constants::tileTypeDirectionStart, Constants::tileTypeDirectionEnd);
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
        return "up-left";
    case 10: // 1010
        return "up-right";

    default:
        return "invalid direction";
        break;
    }
}

 bool Utils::tileBlockedMoveUp(int tileValue)
{
    return extractBits(tileValue, Constants::tileTypeBlockedMoveUp, Constants::tileTypeBlockedMoveUp);
}

 bool Utils::tileBlockedMoveDown(int tileValue)
{
    return Utils::extractBits(tileValue, Constants::tileTypeBlockedMoveDown, Constants::tileTypeBlockedMoveDown);
}

 bool Utils::tileBlockedMoveLeft(int tileValue)
{
    return Utils::extractBits(tileValue, Constants::tileTypeBlockedMoveLeft, Constants::tileTypeBlockedMoveLeft);
}

 bool Utils::tileBlockedMoveRight(int tileValue)
{
    return extractBits(tileValue, Constants::tileTypeBlockedMoveRight, Constants::tileTypeBlockedMoveRight);
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
