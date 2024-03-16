// #ifndef TILE_H
// #define TILE_H
// #include "Player.cpp"
// #include <vector>

// class Tile {
// public:
    
//     bool isStandable;
//     Tile* escalatorTo;
//     Tile* tileAbove;
//     Tile* tileBelow;
//     Tile* tileToRight;
//     Tile* tileToLeft;
//     int tileType;

//     // Constructor
//     Tile(int tileType);

//     // Method to get plausible target tiles
//     Tile* getPlausibleTargetTiles(MovementAbility* movementAbility);
// };

// #endif

#include "Tile.h"


class Tile {
public:
    bool isStandable;
    Tile* escalatorTo;
    Tile* tileAbove;
    Tile* tileBelow;
    Tile* tileToRight;
    Tile* tileToLeft;
    int tileType;

    // Constructor
    Tile(int tileType);

    // Method to get plausible target tiles
    std::vector<Tile*> getPlausibleTargetTiles(MovementAbility* movementAbility);
};

Tile::Tile( int tileType) {
    
    this->tileType = tileType;
    this->escalatorTo = nullptr; // Using nullptr instead of null
}

std::vector<Tile*> Tile::getPlausibleTargetTiles(MovementAbility* movementAbility) {
    Tile *temp;
    std::vector<Tile*> plausibleTiles;

    if(movementAbility->canMoveDown&&this->tileBellow!=NULL){
        plausibleTiles.push_back(this->tileBelow);
        temp = this->tileBelow;
        while(temp->tileBelow!=NULL){
            temp.push_back(temp->tileBelow);
            temp = temp->tileBelow;
        }
    }
    if(movementAbility->canMoveUp&&this->tileAbove!=NULL){
        plausibleTiles.push_back(this->tileAbove);
        temp = this->tileAbove;
        while(temp->tileAbove!=NULL){
            temp.push_back(temp->tileAbove);
            temp = temp->tileAbove;
        }
    }
    if(movementAbility->canMoveRight&&this->tileToRight!=NULL){
        plausibleTiles.push_back(this->tileToRight);
        temp = this->tileToRight;
        while(temp->tileToRight!=NULL){
            temp.push_back(temp->tileToRight);
            temp = temp->tileToRight;
        }
    }
    if(movementAbility->canMoveLeft&&this->tileToLeft!=NULL){
        plausibleTiles.push_back(this->tileToLeft);
        temp = this->tileToLeft;
        while(temp->tileToLeft!=NULL){
            temp.push_back(temp->tileToLeft);
            temp = temp->tileToLeft;
        }
    }
    return possibleTiles;
    
}

