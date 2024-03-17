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

PreFieldPiece::PreFieldPiece(int** tiles, int size) {
    this->size = size;
    this->tiles = new int*[size];
    for (int i = 0; i < size; i++) {
        this->tiles[i] = new int[size];
        for (int j = 0; j < size; j++) {
            this->tiles[i][j] = tiles[i][j];
        }
    }
}



void PreFieldPiece::rotateLeft() {
    int** newTiles = new int*[size];
    for (int i = 0; i < size; i++) {
        newTiles[i] = new int[size];
        for (int j = 0; j < size; j++) {
            newTiles[i][j] = tiles[size - 1 - j][i];
        }
    }

    // Deallocate the old memory
    for (int i = 0; i < size; i++) {
        delete[] tiles[i];
    }
    delete[] tiles;

    // Assign the new array
    tiles = newTiles;
}

void PreFieldPiece::rotateRight() {
    rotateLeft();
    rotateLeft();
    rotateLeft();
}

int** PreFieldPiece::getData(){
    return this->tiles;
}
int PreFieldPiece::getSize(){
    return this->size;
}
#endif