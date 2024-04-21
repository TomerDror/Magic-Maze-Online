    #ifndef CHARACTER_H
    #define CHARACTER_H
    #include "PreFieldPiece.h"
    #include "FieldPiece.h"
    #include "Tile.h"
    #include "Player.h"
    #include "MovementAbility.h"
    #include <string>
    #include <vector>

    class Character{
        public:

            std::string name;
            FieldPiece *fieldPieceOn;
            Tile *tileOn;
            Character(std::string, FieldPiece *startingPieceField,Tile *startingTile);
            std::vector<Tile*> getPlausibleTargetTiles(MovementAbility *playerMovementAbility);
            void move(Tile* target,MovementAbility *playerMovementAbility);
            void openFieldPiece(MovementAbility *playerMovementAbility);
            bool isFieldPieceAlreadyExist(std::string diraction, FieldPiece *fieldPiece);

    };

    #endif 
