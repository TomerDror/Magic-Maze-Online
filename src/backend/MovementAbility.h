#ifndef MOVEMENT_ABILITY_H
#define MOVEMENT_ABILITY_H
typedef struct 
{
    bool canMoveUp;
    bool canMoveDown;
    bool canMoveLeft;
    bool canMoveRight;
    bool canUseEscalator;
    bool canUsePortals;
    bool canOpenFieldPiece;
}MovementAbility;

#endif
