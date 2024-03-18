#ifndef PLAYER_H
#define PLAYER_H

#include "MovementAbility.h" // Include MovementAbility header file

class Player {
public:
    MovementAbility* movementAbility;

    // Constructor
    Player(bool canMoveUp,
           bool canMoveDown,
           bool canMoveLeft,
           bool canMoveRight,
           bool canUseEscalator,
           bool canUsePortals,
           bool canOpenFieldPiece);
};

#endif

