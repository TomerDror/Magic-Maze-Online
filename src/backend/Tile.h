#ifndef TILE_H
#define TILE_H

#include "Player.h"
#include <vector>
#include <iostream>
#include "Field.h"
class Tile
{
    public:
        bool isStandable;
        Tile *escalatorTo;
        Tile *tileAbove;
        Tile *tileBellow;
        Tile *tileToRight;
        Tile *tileToLeft;
        int tileType;

        Tile(int tileType);

        std::vector<Tile *> getPlausibleTargetTiles(MovementAbility *movementAbility);
        

    int getTileFeature();
    int getTileColor();
    
    
};

#endif
