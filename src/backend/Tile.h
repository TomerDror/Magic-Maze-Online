#ifndef TILE_H
#define TILE_H

#include "Player.h" // Include the header file, not the .cpp file
#include <vector>


class Tile {
public:
    bool isStandable;
    Tile* escalatorTo;
    Tile* tileAbove;
    Tile* tileBellow;
    Tile* tileToRight;
    Tile* tileToLeft;
    int tileType;

    // Constructor
    Tile(int tileType);

    // Method to get plausible target tiles
    std::vector<Tile*> getPlausibleTargetTiles(MovementAbility* movementAbility);
};

#endif
