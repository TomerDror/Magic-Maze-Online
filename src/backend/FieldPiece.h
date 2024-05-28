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
    int x;
    int y;
    Tile *tile;
    Tile *entrance;
    std::vector<Tile *> openings;

    // FieldPiece(PreFieldPiece *preFieldPiece);
    FieldPiece(int x, int y,Field *field, PreFieldPiece *preFieldPiece);

    FieldPiece *leftPiece;
    FieldPiece *rightPiece;
    FieldPiece *upPiece;
    FieldPiece *downPiece;

private:
    void locateTile(Tile **returnTile, int x, int y, int **field,int size, std::unordered_map<int, Tile *> *umap_ptr,bool blockedInDirection);
};
#endif