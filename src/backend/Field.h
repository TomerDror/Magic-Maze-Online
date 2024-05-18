#ifndef FIELD_H
#define FIELD_H
class Tile;
class Character;
class FieldPiece;
class PreFieldPiece;

#include "FieldPiece.h"
#include "PreFieldPiece.h"
#include "Character.h"
#include "Tile.h"
#include <queue>

class Field
{
public:
    static Field &getInstance();
    bool isTileVacated(Tile *tile);
    Character *getGreenCharacter();
    Character *getPurpleCharacter();
    Character *getYellowCharacter();
    Character *getOrangeCharacter();
    FieldPiece *getCenterPiece();
    std::vector<Tile*> greenPortals; 
    std::vector<Tile*> purplePortals; 
    std::vector<Tile*> yellowPortals; 
    std::vector<Tile*> orangePortals; 
    std::queue<int> futureFieldPieces;
    PreFieldPiece allFieldPieces[10];
    Field(PreFieldPiece fieldPiece);


// private:
    // Field(FieldPiece *\fieldPiece);
    static Field *instance;
    FieldPiece *centerPiece;
    Character *greenCharacter;
    Character *purpleCharacter;
    Character *yellowCharacter;
    Character *orangeCharacter;
};

#endif