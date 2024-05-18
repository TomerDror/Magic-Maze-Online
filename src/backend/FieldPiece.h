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
#include <vector>

class FieldPiece
{
public:

    Tile *tile;
    Tile *entrance;
    std::vector<Tile*> exits;

 
    FieldPiece(PreFieldPiece *preFieldPiece);
    FieldPiece(Field *field, PreFieldPiece *preFieldPiece);

    FieldPiece *leftPiece;
    FieldPiece *rightPiece;
    FieldPiece *upPiece;
    FieldPiece *downPiece;
private:
    void locateTile(std::unordered_map<int, Tile *> *umap_ptr, int x, int y, Tile *tileInDirection, int **field);



};
#endif