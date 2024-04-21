#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Constants.h"
#include <iostream>

class Utils {
public:
    static void setTileFeature(int *tileValue, int feature);

    static void rotateDiractionLeft(int *tileValue);
    static void setDiraction(int *tileValue, int diractionBitwise);
    static std::string getTileFeature(int tileValue);
    static std::string getTileColor(int tileValue);
    static int getDirectionBitwise(int tileValue);
    static std::string getDirection(int tileValue);
    static bool tileBlockedMoveUp(int tileValue);
    static bool tileBlockedMoveDown(int tileValue);
    static bool tileBlockedMoveLeft(int tileValue);
    static bool tileBlockedMoveRight(int tileValue);
    void  static setTileBlockedMoveUp(int *tileValue, bool blockedUp);
    void  static setTileBlockedMoveDown(int *tileValue, bool blockedDown);
    void  static setTileBlockedMoveLeft(int *tileValue, bool blockedLeft);
    void  static setTileBlockedMoveRight(int *tileValue, bool blockedRight);
    int static setBits(int num,int value, int start, int end);
    static int extractBits(int num, int start, int end);
};

#endif /* UTILS_H */
