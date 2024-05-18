

#include "FieldPiece.h"

FieldPiece::FieldPiece(PreFieldPiece *preFieldPiece)
{
    FieldPiece(&Field::getInstance(), preFieldPiece);
}
FieldPiece::FieldPiece(Field *playingField, PreFieldPiece *preFieldPiece)
{
    int size = preFieldPiece->getSize();
    int **field = preFieldPiece->getData();
    std::unordered_map<int, Tile *> umap;
    bool isFirst = true;
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            // std::cout<<x<<", "<<y;
            if (field[y][x] != 0)
            {
                Tile *currTile;

                if (umap.find(10 * y + x) == umap.end())
                {
                    // std::cout<<" creating tile ";

                    currTile = new Tile(field[y][x]);
                    umap[10 * y + x] = currTile;
                    if (isFirst)
                    {
                        isFirst = false;
                        this->tile = currTile;
                    }
                }
                else
                {
                    currTile = umap[10 * y + x];
                }

                this->locateTile(&(currTile->tileToRight), currTile,  x+1,  y, field, &umap);
                this->locateTile(&(currTile->tileToLeft ), currTile,  x-1,  y, field, &umap);
                this->locateTile(&(currTile->tileBellow ), currTile,  x,  y+1, field, &umap);
                this->locateTile(&(currTile->tileAbove  ), currTile,  x,  y-1, field, &umap);

               

                if (Utils::getTileFeature(field[y][x]) == "entrance")
                {
                    this->entrance = currTile;
                }
                // std::cout << Utils::getTileFeature(field[y][x])<<" " <<" "<<x<<","<<y<< "\n";
                if (Utils::getTileFeature(field[y][x]) == "escalator")
                {

                    int direction = Utils::getDirectionBitwise(field[y][x]);
                    int escalatorX = x;
                    int escalatorY = y;
                    escalatorY -= ((direction >> 3) & 1) - ((direction >> 2) & 1);
                    escalatorX -= ((direction >> 1) & 1) - ((direction >> 0) & 1);
                    Tile *escalatorTile;
                    // std::cout<<"esc x,y "<< escalatorX<<","<<escalatorY<<"/n";
                    if (umap.find(10 * (escalatorY) + escalatorX) == umap.end())
                    {
                        // std::cout << " ,cr esc";

                        escalatorTile = new Tile(field[escalatorY][escalatorX]);
                        umap[10 * (escalatorY) + escalatorX] = escalatorTile;
                    }
                    else
                    {
                        // std::cout << " ,con esc";
                        escalatorTile = umap[10 * (escalatorY) + escalatorX];
                    }
                    currTile->escalatorTo = escalatorTile;
                }
                // std::cout<<"type "<< currTile->tileType<<" type ";
                // std::cout<<" yay1 ";
                if (Utils::getTileFeature(field[y][x]) == "portal")
                {
                    // std::cout<<" yay2 ";
                    if (Utils::getTileColor(field[y][x]) == "green")
                    {
                        // std::cout<<" yay3" ;
                        playingField->greenPortals.push_back(currTile);
                    }
                    if (Utils::getTileColor(field[y][x]) == "purple")
                    {
                        playingField->purplePortals.push_back(currTile);
                    }
                    if (Utils::getTileColor(field[y][x]) == "orange")
                    {
                        playingField->orangePortals.push_back(currTile);
                    }
                    if (Utils::getTileColor(field[y][x]) == "yellow")
                    {
                        playingField->yellowPortals.push_back(currTile);
                    }
                }
            }
            // std::cout<<"\n";
        }
    }
}
void FieldPiece::locateTile(Tile **returnTile, Tile *currTile, int x, int y, int **field, std::unordered_map<int, Tile *> *umap_ptr)
{
    if (x >= 1 && field[y][x - 1] != 0 && !Utils::tileBlockedMoveUp(currTile->tileType))
    {
        Tile *tileInDirection;
        if ((*umap_ptr).find(10 * (y) + x) == (*umap_ptr).end())
        {
            // std::cout<<" ,cr up";

            tileInDirection = new Tile(field[y][x]);
            (*umap_ptr)[10 * (y) + x] = tileInDirection;

            if (Utils::getTileFeature(tileInDirection->tileType) == "exit")
            {
                this->exits.push_back(tileInDirection);
            }
        }
        else
        {
            // std::cout<<" ,con up";
            tileInDirection = (*umap_ptr)[10 * (y) + x];
        }
        (*returnTile) = tileInDirection;
    }
}