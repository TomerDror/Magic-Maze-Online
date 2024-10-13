#include "Field.h"
// #include <map>
// #include <iostream>
Field *Field::instance = nullptr;

Field *Field::getInstance()
{
    // std::cout << "b";
    if (instance == nullptr)
    {

        // std::cout << "pls\n";
        instance = new Field();
        // std::cout << " conversation " << instance->centerPiece->openings.size() << "\n";
        // std::cout << "nooo\n";
    }
    return instance;
}

Field::Field()
{
    int size = 4;
    std::string filename = "../../DB/FieldPieces.txt";

    for (int i = 1; i <= 5; i++)
    {
        int **allFieldPiecesMap = Utils::readDatabaseFromFile(filename, i);
        // std::cout << "\n hello"<< (*(*allFieldPieces));
        this->allFieldPieces[i] = new PreFieldPiece(allFieldPiecesMap, size);
    }
    // std::cout << "d";

    // std::cout << "e";
    PreFieldPiece *preFieldPiece = allFieldPieces[1];
    // preFieldPiece->rotateLeft();
    // int **arr = preFieldPiece->getData();
    // std::cout <<" wow "<<arr[0][1] <<"\n ";
    // for (int i = 0; i < 4; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //     {
    //         std::cout <<arr[i][j]<<" ";
    //         /* code */
    //     }
    //     std::cout <<"\n";
    //     /* code */
    // }

    FieldPiece *fieldPiece = new FieldPiece(0, 0, this, preFieldPiece);

    centerPiece = fieldPiece;
    this->openedFieldPieces.push_back(centerPiece);

    // this->purpleCharacter = new Character("purple", this->centerPiece, this->centerPiece->tile);
    // this->greenCharacter = new Character("green", this->centerPiece, this->centerPiece->tile);
    // this->orangeCharacter = new Character("orange", this->centerPiece, this->centerPiece->tile);
    // this->yellowCharacter = new Character("yellow", this->centerPiece, this->centerPiece->tile);
    this->greenCharacter = new Character("green", this->centerPiece, this->centerPiece->tile->tileToRight->tileToRight->tileBellow->tileBellow);
    this->yellowCharacter = new Character("yellow", this->centerPiece, this->centerPiece->tile->tileToRight->tileBellow->tileBellow);
    this->orangeCharacter = new Character("orange", this->centerPiece, this->centerPiece->tile->tileToRight->tileToRight->tileBellow);
    this->purpleCharacter = new Character("purple", this->centerPiece, this->centerPiece->tile->tileToRight->tileBellow);
}

bool Field::isTileVacated(Tile *tile)
{

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
