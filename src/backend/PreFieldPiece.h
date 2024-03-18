#ifndef PREFIELDPIECE_H
#define PREFIELDPIECE_H
class PreFieldPiece {
private:
    int** tiles;
    int size;

public:
    PreFieldPiece(int** tiles, int size);
    void rotateLeft();
    void rotateRight();
    int** getData();
    int getSize();
};

#endif