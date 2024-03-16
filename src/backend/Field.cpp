#include "FieldPiece.cpp"
#include "Character.cpp"

class Field{
    public:
        static FieldPiece *centerPiece;
        static Character *greenCharacter;
        static Character *purpleCharacter;
        static Character *yellowCharacter;
        static Character *orangeCharacter;
        bool isTileVacated(Tile* tile){
            return *tile == (greenCharacter->tileOn) || *tile == (purpleCharacter->tileOn) || *tile == (yellowCharacter->tileOn) || *tile == (orangeCharacter->tileOn) 
        }
};

