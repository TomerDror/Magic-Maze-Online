#ifndef CHARACTER_H
#define CHARACTER_H
#include "PreFieldPiece.cpp"
#include "FieldPiece.cpp"
#include "Tile.cpp"
#include "Player.cpp"
#include <string>

class Character{
    public:
        std::string name;
        FieldPiece *fieldPieceOn;
        Tile *tileOn;
        Character(std::string, FieldPiece *startingPieceField,Tile *startingTile,MovementAbility *movementAbility);
        Tile *getPlausibleTargetTiles(MovementAbility *playerMovementAbility){
            if(tileOn == NULL){
                return NULL;
            }
            return this->(tileOn->getPlausibleTargetTiles(MovementAbility));
        }

};

    Character::Character(std::string name, FieldPiece *startingFieldPiece,Tile *startingTile){
        this->name = name;
        this->fieldPieceOn = startingFieldPiece;
        this->tileOn = startingTile;
    }




#endif // CHARACTER_H
