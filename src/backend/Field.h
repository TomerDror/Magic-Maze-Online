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
    std::queue<PreFieldPiece*> futureTiles;

private:
    static Field *instance;
    FieldPiece *centerPiece;
    Character *greenCharacter;
    Character *purpleCharacter;
    Character *yellowCharacter;
    Character *orangeCharacter;
    Field(PreFieldPiece *fieldPiece);
    Field(FieldPiece *fieldPiece);
};

#endif