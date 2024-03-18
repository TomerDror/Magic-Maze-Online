

#include "Tile.h"



Tile::Tile( int tileType) {
    
    this->tileType = tileType;
    this->escalatorTo = nullptr; // Using nullptr instead of null
}

std::vector<Tile*> Tile::getPlausibleTargetTiles(MovementAbility* movementAbility) {
    Tile *temp;
    std::vector<Tile*> plausibleTiles;

    if(movementAbility->canMoveDown&&this->tileBellow!=nullptr){
        plausibleTiles.push_back(this->tileBellow);
        temp = this->tileBellow;
        while(temp->tileBellow!=NULL){
            plausibleTiles.push_back(temp->tileBellow);
            temp = temp->tileBellow;
        }
    }
    if(movementAbility->canMoveUp&&this->tileAbove!=nullptr){
        plausibleTiles.push_back(this->tileAbove);
        temp = this->tileAbove;
        while(temp->tileAbove!=NULL){
            plausibleTiles.push_back(temp->tileAbove);
            temp = temp->tileAbove;
        }
    }
    if(movementAbility->canMoveRight&&this->tileToRight!=nullptr){
        plausibleTiles.push_back(this->tileToRight);
        temp = this->tileToRight;
        while(temp->tileToRight!=NULL){
            plausibleTiles.push_back(temp->tileToRight);
            temp = temp->tileToRight;
        }
    }
    if(movementAbility->canMoveLeft&&this->tileToLeft!=nullptr){
        plausibleTiles.push_back(this->tileToLeft);
        temp = this->tileToLeft;
        while(temp->tileToLeft!=NULL){
            plausibleTiles.push_back(temp->tileToLeft);
            temp = temp->tileToLeft;
        }
    }
    return plausibleTiles;
    
}

