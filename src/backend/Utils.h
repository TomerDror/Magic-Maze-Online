#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Constants.h"
#include <iostream>
// class Constants {
// public:
//     static const int  tileTypeFeatureStart = 5 ;
//     static const int tileTypeFeatureEnd = 8;
//     static const int tileTypeColorStart = 9;
//     static const int tileTypeColorEnd = 10;
//     static const int tileTypeDirectionStart  = 11;
//     static const int tileTypeDirectionEnd = 14;
//     static const int tileTypeBlockedMoveUp = 1;
//     static const int tileTypeBlockedMoveDown = 2;
//     static const int tileTypeBlockedMoveLeft = 3;
//     static const int tileTypeBlockedMoveRight =4;
// };

class Utils {
public:
    static std::string getTileFeature(int tileValue);
    static std::string getTileColor(int tileValue);
    static int getDirectionBitwise(int tileValue);
    static std::string getDirection(int tileValue);
    static bool tileBlockedMoveUp(int tileValue);
    static bool tileBlockedMoveDown(int tileValue);
    static bool tileBlockedMoveLeft(int tileValue);
    static bool tileBlockedMoveRight(int tileValue);
    static int extractBits(int num, int start, int end);
};

#endif /* UTILS_H */
