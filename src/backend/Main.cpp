#include "Field.h"
#include "Character.h"
#include "PreFieldPiece.h" 
#include "FieldPiece.h"
#include "Player.h"
#include "Tile.h"
#include <iostream>
int main() {
    // int size = 4;
    //     int** tiles = new int*[size];
    //     for (int i = 0; i < size; ++i)
    //     {
    //         tiles[i] = new int[size];
    //         for (int j = 0; j < size; ++j)
    //         {
    //             tiles[i][j] = 4*i+j;
    //         }
    //     }
    //     PreFieldPiece preFieldPiece(tiles, size);
    //     FieldPiece FieldPiece
    


    Field& fieldInstance = Field::getInstance();
    // if(fieldInstance.isTileVacated(fieldInstance.getCenterPiece()->tile)){
    // std::cout<<"yay";
    // }
    // else{
    std::cout<<"oof";
    // }
    return 0;
}
