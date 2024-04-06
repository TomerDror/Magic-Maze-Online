#ifndef FIELDPIECE
#define FIELDPIECE

class Tile;
class PreFieldPiece;
#include "Utils.h"
#include <iostream>
#include "PreFieldPiece.h"
#include "Tile.h"
#include <unordered_map> 
#include <stdexcept>

class FieldPiece
{
public:

    Tile *tile;
    FieldPiece *getLeftPiece();
    FieldPiece *getRightPiece();
    FieldPiece *getUpPiece();
    FieldPiece *getDownPiece();
    FieldPiece(PreFieldPiece *preFieldPiece);
    private:
    FieldPiece *leftPiece;
    FieldPiece *rightPiece;
    FieldPiece *upPiece;
    FieldPiece *downPiece;
};
#endif