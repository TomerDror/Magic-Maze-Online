#include "Field.h"
#include <iostream>
Field *Field::instance = nullptr;

Field &Field::getInstance()
{
    std::cout << "b";
    if (instance == nullptr)
    {
        int size = 4;
        // int** tiles = new int*[size];
        // for (int i = 0; i < size; ++i)
        // {
        //     tiles[i] = new int[size];
        //     for (int j = 0; j < size; ++j)
        //     {
        //         tiles[i][j] = 100 +4*i+j+1;
        //     }
        // }

        // int tiles** = {
        // {120, 30, 655, 340},
        // {396, 31, 31, 852},
        // {600, 31, 23, 6240},
        // {88, 911, 10341, 0}
        // };
        const int rows = 4;
        const int cols = 4;

        // Dynamically allocate memory for the tiles
        int **tiles = new int *[rows];
        for (int i = 0; i < rows; ++i)
        {
            tiles[i] = new int[cols];
        }

        // Assign values to the tiles
        tiles[0][0] = 100120;
        tiles[0][1] = 200030;
        tiles[0][2] = 300655;
        tiles[0][3] = 400340;
        tiles[1][0] = 500396;
        tiles[1][1] = 600031;
        tiles[1][2] = 700031;
        tiles[1][3] = 800852;
        tiles[2][0] = 900600;
        tiles[2][1] = 1000031;
        tiles[2][2] = 1100023;
        tiles[2][3] = 1206240;
        tiles[3][0] = 1300088;
        tiles[3][1] = 1400911;
        tiles[3][2] = 1509317;
        tiles[3][3] = 0;

        PreFieldPiece preFieldPiece(tiles, size);

        // preFieldPiece.rotateLeft();
        std::cout << preFieldPiece.size;
        instance = new Field(preFieldPiece);
        std::cout << "nooo\n";
    }
    return *instance;
}

Field::Field(PreFieldPiece fieldPiece)
{
    std::cout<<"asf";
    FieldPiece fp(this, &fieldPiece);
    std::cout<<"asf";
    centerPiece = &fp;

    this->greenCharacter = new Character("green", this->centerPiece, this->centerPiece->tile->tileToRight->tileBellow);
    this->purpleCharacter = new Character("purple", this->centerPiece, this->centerPiece->tile->tileToRight->tileToRight->tileBellow);
    this->yellowCharacter = new Character("yellow", this->centerPiece, this->centerPiece->tile->tileToRight->tileBellow->tileBellow);
    this->orangeCharacter = new Character("orange", this->centerPiece, this->centerPiece->tile->tileToRight->tileToRight->tileBellow->tileBellow);

}


bool Field::isTileVacated(Tile *tile)
{
    // Check if any of the characters occupy the tile
    // if( this->greenCharacter == nullptr)
    //     std::cout<<"wow\n";
    // std::cout<< (tile != greenCharacter->tileOn)<<
    //         (tile != purpleCharacter->tileOn)<<
    //         (tile != yellowCharacter->tileOn)<<
    //         (tile != orangeCharacter->tileOn);
    // std::cout <<"\n";
    return (tile != greenCharacter->tileOn &&
            tile != purpleCharacter->tileOn &&
            tile != yellowCharacter->tileOn &&
            tile != orangeCharacter->tileOn);
}

Character *Field::getGreenCharacter()
{

    return greenCharacter;
}

Character *Field::getPurpleCharacter()
{
    return purpleCharacter;
}

Character *Field::getYellowCharacter()
{
    return yellowCharacter;
}

Character *Field::getOrangeCharacter()
{
    return orangeCharacter;
}

FieldPiece *Field::getCenterPiece()
{
    return centerPiece;
}
