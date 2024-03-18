#include "Field.h"
#include <iostream>
Field* Field::instance = nullptr;

 Field &Field::getInstance()
{
    if (instance == nullptr)
    {
        int size = 4;
        int** tiles = new int*[size];
        for (int i = 0; i < size; ++i)
        {
            tiles[i] = new int[size];
            for (int j = 0; j < size; ++j)
            {
                tiles[i][j] = 100 +4*i+j+1;
            }
        }
        PreFieldPiece preFieldPiece(tiles, size);

    
        instance = new Field(&preFieldPiece);
    }
    return *instance;
}

Field::Field(PreFieldPiece *fieldPiece)
{
    centerPiece = new FieldPiece(fieldPiece);
 

    this->greenCharacter = new Character("green character",this->centerPiece,this->centerPiece->tile->tileToRight);
    this->purpleCharacter = new Character("purple character",this->centerPiece,this->centerPiece->tile->tileToRight->tileToRight->tileBellow);
    this->yellowCharacter = new Character("yellow character",this->centerPiece,this->centerPiece->tile->tileToRight->tileBellow->tileBellow);
    this->orangeCharacter = new Character("orange character",this->centerPiece,this->centerPiece->tile->tileToRight->tileToRight->tileBellow->tileBellow);
}

Field::Field(FieldPiece *fieldPiece)
{
    this->centerPiece = fieldPiece;
    greenCharacter = nullptr;
    this->purpleCharacter = nullptr;
    this->yellowCharacter = nullptr;
    this->orangeCharacter = nullptr;
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

Character* Field::getGreenCharacter()
{
    return greenCharacter;
}

Character* Field::getPurpleCharacter()
{
    return purpleCharacter;
}

Character* Field::getYellowCharacter()
{
    return yellowCharacter;
}

Character* Field::getOrangeCharacter()
{
    return orangeCharacter;
}

FieldPiece* Field::getCenterPiece()
{
    return centerPiece;
}
