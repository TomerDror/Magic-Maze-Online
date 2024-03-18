#include "Player.h"
// class Player
// {
// public:
//     MovementAbility *movementAbility;
//     Player(bool canMoveUp,
//            bool canMoveDown,
//            bool canMoveLeft,
//            bool canMoveRight,
//            bool canUseEscalator,
//            bool canUsePortals,
//            bool canOpenFieldPiece);
// };

Player::Player(bool canMoveUp,
       bool canMoveDown,
       bool canMoveLeft,
       bool canMoveRight,
       bool canUseEscalator,
       bool canUsePortals,
       bool canOpenFieldPiece)
{
    this->movementAbility = new MovementAbility(); 
    this->movementAbility->canMoveUp = canMoveUp;
    this->movementAbility->canMoveDown = canMoveDown;
    this->movementAbility->canMoveLeft = canMoveLeft;
    this->movementAbility->canMoveRight = canMoveRight;
    this->movementAbility->canUseEscalator = canUseEscalator;
    this->movementAbility->canUsePortals = canUsePortals;
    this->movementAbility->canOpenFieldPiece = canOpenFieldPiece;
}

