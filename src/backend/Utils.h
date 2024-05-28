#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <queue>
#include "Constants.h"
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>


class Utils
{
public:
    static void setTileFeature(int *tileValue, int feature);
    static void rotateDirectionLeft(int *tileValue);

    static void setEscalatorDirection(int *tileValue, int directionBitwise);
    static int getEscalatorDirectionBitwise(int tileValue);
    static std::string getEscalatorDirection(int tileValue);

    static std::string getTileFeature(int tileValue);
    static std::string getTileColor(int tileValue);
    static int getDirectionBitwise(int tileValue);
    static std::string getDirection(int tileValue);
    static void setDirection(int *tileValue, int directionBitwise);
    static bool tileBlockedMoveUp(int tileValue);
    static bool tileBlockedMoveDown(int tileValue);
    static bool tileBlockedMoveLeft(int tileValue);
    static bool tileBlockedMoveRight(int tileValue);
    void static setTileBlockedMoveUp(int *tileValue, bool blockedUp);
    void static setTileBlockedMoveDown(int *tileValue, bool blockedDown);
    void static setTileBlockedMoveLeft(int *tileValue, bool blockedLeft);
    void static setTileBlockedMoveRight(int *tileValue, bool blockedRight);
    int static setBits(int num, int value, int start, int end);
    static int extractBits(int num, int start, int end);
    static std::queue<int> createQueueFromVector(const std::vector<int> &vec);
    static  int ** readDatabaseFromFile(const std::string &filePath,int targetID) ;

    };

#endif /* UTILS_H */
