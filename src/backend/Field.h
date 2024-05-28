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
#include <map>

#include <queue>

class Field
{
public:
    static Field *getInstance();
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
    PreFieldPiece *allFieldPieces[10] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    Field();
    std::vector<FieldPiece*> openedFieldPieces;

// private:
    // Field(FieldPiece *\fieldPiece);
    static Field *instance ;
    FieldPiece *centerPiece;
    Character *greenCharacter;
    Character *purpleCharacter;
    Character *yellowCharacter;
    Character *orangeCharacter;
};
// Field* Field::instance = nullptr;

#endif