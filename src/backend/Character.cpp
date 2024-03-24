
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


// void Character::move(Tile *tile,MovementAbility *playerMovementAbility){
//     std::vector<Tile*> plausibleTiles = getPlausibleTargetTiles(playerMovementAbility);
//     if(std::find(plausibleTiles.begin(), plausibleTiles.end(), tile)!= plausibleTiles.end()){
//         this->tileOn = tile;
//     }
// }

void Character::move(Tile *tile, MovementAbility *playerMovementAbility) {
    std::vector<Tile*> plausibleTargetTiles = getPlausibleTargetTiles(playerMovementAbility);
    bool isValidMove = false;
    for (Tile* targetTile : plausibleTargetTiles) {
        if (targetTile == tile) {
            isValidMove = true;
            break;
        }
    }
    // std::cout<<"\n"<< isValidMove<<"\n";
    if (isValidMove) {
        this->tileOn=tile;
    }
}