#ifndef PREFIELDPIECE_H
#define PREFIELDPIECE_H
#include "Constants.h"
class PreFieldPiece {
private:
    int** tiles;
    int size;

    int getTileFeature(int tileValue);
    int getTileColor(int tileValue);
    
    bool canTileMoveUp(int tileValue);
    bool canTileMoveDown(int tileValue);
    bool canTileMoveLeft(int tileValue);
    bool canTileMoveRight(int tileValue);

public:
    PreFieldPiece(int** tiles, int size);
    void rotateLeft();
    void rotateRight();
    int** getData();
    int getSize();
    
};

#endif