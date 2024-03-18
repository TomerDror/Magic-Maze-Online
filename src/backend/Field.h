#ifndef FIELD_H
#define FIELD_H
#include "FieldPiece.h"
#include "PreFieldPiece.h"
#include "Character.h"


class Field{
    public:
        static Field &getInstance();
        bool isTileVacated(Tile* tile);
        Character *getGreenCharacter();
        Character *getPurpleCharacter();
        Character *getYellowCharacter();
        Character *getOrangeCharacter();
        FieldPiece *getCenterPiece();
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