
// #ifndef FIELDPIECE_H
// #define FIELDPIECE_H

// #include "Tile.cpp"
// #include "PreFieldPiece.cpp"
// #include "MovementAbility.h"

// #include <unordered_map>

// // #include <stdexcept>

// void getFirstNonZero(int **array, int numRows, int numCols, int *x, int *y);

// class FieldPiece
// {
// public:
//     FieldPiece *leftPiece;
//     FieldPiece *rightPiece;
//     FieldPiece *upPiece;
//     FieldPiece *downPiece;

//     Tile *tile;

//     FieldPiece(PreFieldPiece *preFieldPiece);
// };

// #endif
// FieldPiece::FieldPiece(PreFieldPiece *preFieldPiece)
// {
//     int size = preFieldPiece->getSize();
//     int **field = preFieldPiece->getData();
//     int x, y;
//     // getFirstNonZero(preFieldPiece->getData(), size,size,&x,&y);
//     // this->tile = new Tile(preFieldPiece->getData()[x][y]);
//     std::unordered_map<int, Tile*> umap;

//     for (y = 0; y < size; y++)
//         for (x = 0; x < size; x++)
//         {
//             if (field[y][x] != 0)
//             {
//                 Tile *currTile;
//                 Tile *rightTile;
//                 // Tile *leftTile;
//                 // Tile *aboveTile;
//                 // Tile *belowTile;
                
//                 if (umap.find(10 * y + x) == umap.end())
//                 {
//                     currTile = new Tile(field[y][x]);
//                     umap[10 * y + x] = currTile;
//                 }
//                 else
//                 {
//                     currTile = umap.find(10 * y + x);
//                 }

//                 if (x < size - 1 && field[y][x - 1] != 0)
//                 {
                    
//                     if(umap.find(10 * y + x+1) == umap.end()){
//                         rightTile = new Tile(field[y][x+1]);
//                         umap[10 * y + x] = rightTile;
//                     }
//                     else{
//                         rightTile = umap.find(10 * y + x+1);
//                     }
//                     *currTile->tileToRight = rightTile;
//                 }

//             }
//         }
// }
// // void getFirstNonZero(int** array, int numRows, int numCols, int *x ,int *y) {
// //     for (int i = 0; i < numRows; ++i) {
// //         for (int j = 0; j < numCols; ++j) {
// //             if (array[i][j] != 0) {
// //                 *x = i;
// //                 *y = j;
// //                 return;
// //             }
// //         }
// //     }
// //     // throw std::invalid_argument( "empty field piece" );

// //     return ; // Return 0 if no non-zero element found
// // }
#ifndef FIELDPIECE_H
#define FIELDPIECE_H

#include "Tile.cpp" // Assuming Tile's declaration is in Tile.h
#include "PreFieldPiece.cpp" // Assuming PreFieldPiece's declaration is in PreFieldPiece.h
#include <unordered_map>

class FieldPiece
{
public:
    FieldPiece *leftPiece;
    FieldPiece *rightPiece;
    FieldPiece *upPiece;
    FieldPiece *downPiece;

    Tile *tile;

    FieldPiece(PreFieldPiece *preFieldPiece);
};

#endif

#include <iostream> // For std::cout (for debugging)

FieldPiece::FieldPiece(PreFieldPiece *preFieldPiece)
{
    int size = preFieldPiece->getSize();
    int **field = preFieldPiece->getData();
    std::unordered_map<int, Tile *> umap;

    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            if (field[y][x] != 0)
            {
                Tile *currTile;

                if (umap.find(10 * y + x) == umap.end())
                {
                    currTile = new Tile(field[y][x]);
                    umap[10 * y + x] = currTile;
                }
                else
                {
                    currTile = umap[10 * y + x];
                }

                if (x < size - 1 && field[y][x + 1] != 0)
                {
                    Tile *rightTile;

                    if (umap.find(10 * y + x + 1) == umap.end())
                    {
                        rightTile = new Tile(field[y][x + 1]);
                        umap[10 * y + x + 1] = rightTile;
                    }
                    else
                    {
                        rightTile = umap[10 * y + x + 1];
                    }

                    currTile->tileToRight = rightTile; // Assuming tileToRight is a public member of Tile
                }
            }
        }
    }
}
