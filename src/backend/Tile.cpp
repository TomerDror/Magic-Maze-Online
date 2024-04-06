

#include "Tile.h"

Tile::Tile(int tileType)
{

    this->tileType = tileType;
    this->tileAbove = nullptr;
    this->tileToRight = nullptr;
    this->tileToLeft = nullptr;
    this->tileBellow = nullptr;
    this->escalatorTo = nullptr; // Using nullptr instead of null
}

std::vector<Tile *> Tile::getPlausibleTargetTiles(MovementAbility *movementAbility)
{
    Tile *temp;
    std::vector<Tile *> plausibleTiles;
    if (movementAbility->canMoveDown && this->tileBellow != nullptr && Field::getInstance().isTileVacated(this->tileBellow))
    {
        plausibleTiles.push_back(this->tileBellow);
        temp = this->tileBellow;

        while (temp->tileBellow != nullptr && Field::getInstance().isTileVacated(temp->tileBellow))
        {
            plausibleTiles.push_back(temp->tileBellow);
            temp = temp->tileBellow;
        }
    }

    if (movementAbility->canMoveUp && this->tileAbove != nullptr && Field::getInstance().isTileVacated(this->tileAbove))
    {

        plausibleTiles.push_back(this->tileAbove);
        temp = this->tileAbove;
        while (temp->tileAbove != nullptr && Field::getInstance().isTileVacated(temp->tileAbove))
        {
            plausibleTiles.push_back(temp->tileAbove);
            temp = temp->tileAbove;
        }
    }
    if (movementAbility->canMoveRight && this->tileToRight != nullptr && Field::getInstance().isTileVacated(this->tileToRight))
    {

        plausibleTiles.push_back(this->tileToRight);
        temp = this->tileToRight;
        while (temp->tileToRight != nullptr && Field::getInstance().isTileVacated(temp->tileToRight))
        {
            plausibleTiles.push_back(temp->tileToRight);
            temp = temp->tileToRight;
        }
    }
    if (movementAbility->canMoveLeft && this->tileToLeft != nullptr && Field::getInstance().isTileVacated(this->tileToLeft))
    {
        plausibleTiles.push_back(this->tileToLeft);
        temp = this->tileToLeft;
        while (temp->tileToLeft != nullptr && Field::getInstance().isTileVacated(temp->tileToLeft))
        {
            plausibleTiles.push_back(temp->tileToLeft);
            temp = temp->tileToLeft;
        }
    }
    if (movementAbility->canUseEscalator && this->escalatorTo != nullptr && Field::getInstance().isTileVacated(this->escalatorTo)){
            plausibleTiles.push_back(this->escalatorTo);
    }
    return plausibleTiles;
}
