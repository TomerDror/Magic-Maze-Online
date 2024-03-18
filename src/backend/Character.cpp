
#include "Character.h"

// class Character
// {
// public:
//     std::string name;
//     FieldPiece *fieldPieceOn;
//     Tile *tileOn;
//     Character(std::string, FieldPiece *startingPieceField, Tile *startingTile);
//     std::vector<Tile*> getPlausibleTargetTiles(MovementAbility *playerMovementAbility);
// };

Character::Character(std::string name, FieldPiece *startingFieldPiece, Tile *startingTile)
{
    this->name = name;
    this->fieldPieceOn = startingFieldPiece;
    this->tileOn = startingTile;
}

std::vector<Tile*> Character::getPlausibleTargetTiles(MovementAbility *playerMovementAbility)
{
    std::vector<Tile*> plausibleTargetTiles;
    if (this->tileOn != nullptr)
    {
        plausibleTargetTiles = (tileOn)->getPlausibleTargetTiles(playerMovementAbility);
    }
    return plausibleTargetTiles;
}
