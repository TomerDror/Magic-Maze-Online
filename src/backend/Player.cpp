#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
    MovementAbility *movementAbility;
    Player(bool canMoveUp,
           bool canMoveDown,
           bool canMoveLeft,
           bool canMoveRight,
           bool canUseEscalator,
           bool canUsePortals,
           bool canOpenFieldPiece);
};

Player(bool canMoveUp,
       bool canMoveDown,
       bool canMoveLeft,
       bool canMoveRight,
       bool canUseEscalator,
       bool canUsePortals,
       bool canOpenFieldPiece)
{
    MovementAbility MovementAbility;
    MovementAbility.canMoveDown = canMoveDown;
    MovementAbility.canMoveLeft = canMoveLeft;
    MovementAbility.canMoveRight = canMoveRight;
    MovementAbility.canUseEscalator = canUseEscalator;
    MovementAbility.canOpenFieldPiece = canOpenFieldPiece;
    
}

#endif