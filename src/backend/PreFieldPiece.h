#ifndef PREFIELDPIECE_H
#define PREFIELDPIECE_H
#include "Constants.h"
#include "Utils.h"
class PreFieldPiece {
private:
    int** tiles;

    int getTileFeature(int tileValue);
    int getTileColor(int tileValue);
    
    bool canTileMoveUp(int tileValue);
    bool canTileMoveDown(int tileValue);
    bool canTileMoveLeft(int tileValue);
    bool canTileMoveRight(int tileValue);

public:
    int size;
    PreFieldPiece(int** tiles, int size);
    // PreFieldPiece();
    void rotateLeft();
    void rotateRight();
    int** getData();
    int getSize();
    
};

#endif