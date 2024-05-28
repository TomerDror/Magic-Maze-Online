#include "PreFieldPiece.h"

// PreFieldPiece::PreFieldPiece(){
//     size =0;
//     std::cout<<"error! please contact Tomer ";
// }
PreFieldPiece::PreFieldPiece(int **tiles, int size)
{
    // std::cout << "fasdf";
    this->size = size;
    this->tiles = new int *[size];
    // std::cout << "wow";
    // std::cout <<" \n";
    for (int i = 0; i < size; i++)
    {
        this->tiles[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            // std::cout <<" "<< tiles[i][j];
            this->tiles[i][j] = tiles[i][j];
        }
        // std::cout <<" \n";
    }
}

void PreFieldPiece::rotateLeft()
{
    int **newTiles = new int *[size];
    for (int i = 0; i < size; i++)
    {
        newTiles[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            newTiles[i][j] = (tiles[j][size - 1 - i]);
            Utils::rotateDirectionLeft(&newTiles[i][j]);
        }
    }

    // Deallocate the old memory

    // Assign the new array
    tiles = newTiles;
}

void PreFieldPiece::rotateRight()
{
    rotateLeft();
    rotateLeft();
    rotateLeft();
}

int **PreFieldPiece::getData()
{
    return this->tiles;
}
int PreFieldPiece::getSize()
{
    return this->size;
}
// unsigned int PreFieldPiece::extractBits(unsigned int num, unsigned int start, unsigned int end)
// {
//     unsigned int mask = (1 << (end - start + 1)) - 1;
//     mask <<= start;
//     return (num & mask) >> start;
// }
