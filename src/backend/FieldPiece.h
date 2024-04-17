#ifndef FIELDPIECE
#define FIELDPIECE

class Tile;
class PreFieldPiece;
class Field;
#include "Utils.h"
#include <iostream>
#include "PreFieldPiece.h"
#include "Tile.h"
#include <unordered_map> 
#include <stdexcept>
#include "Field.h"

class FieldPiece
{
public:

    Tile *tile;
    Tile *entrance;
    
    FieldPiece *getLeftPiece();
    FieldPiece *getRightPiece();
    FieldPiece *getUpPiece();
    FieldPiece *getDownPiece();
    FieldPiece(PreFieldPiece *preFieldPiece);
    FieldPiece(Field *field, PreFieldPiece *preFieldPiece);
    private:
    FieldPiece *leftPiece;
    FieldPiece *rightPiece;
    FieldPiece *upPiece;
    FieldPiece *downPiece;


};
#endif