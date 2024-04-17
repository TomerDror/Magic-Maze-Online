

#include "FieldPiece.h"

FieldPiece::FieldPiece( PreFieldPiece *preFieldPiece)
{
    FieldPiece(&Field::getInstance(),preFieldPiece);
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

                if (x < size - 1 && field[y][x + 1] != 0)
                {
                    Tile *rightTile;

                    if (umap.find(10 * y + x + 1) == umap.end())
                    {
                        // std::cout<<" ,cr right";

                        rightTile = new Tile(field[y][x + 1]);
                        umap[10 * y + x + 1] = rightTile;
                    }
                    else
                    {
                        // std::cout<<" ,con right";
                        rightTile = umap[10 * y + x + 1];
                    }

                    currTile->tileToRight = rightTile;
                }

                if (x >= 1 && field[y][x - 1] != 0)
                {
                    Tile *leftTile;

                    if (umap.find(10 * y + x - 1) == umap.end())
                    {
                        // std::cout<<" ,cr left";
                        leftTile = new Tile(field[y][x - 1]);
                        umap[10 * y + x - 1] = leftTile;
                    }
                    else
                    {
                        // std::cout<<" ,con left";
                        leftTile = umap[10 * y + x - 1];
                    }

                    currTile->tileToLeft = leftTile;
                }

                if (y < (size - 1) && field[y + 1][x] != 0)
                {
                    Tile *downTile;

                    if (umap.find(10 * (y + 1) + x) == umap.end())
                    {
                        // std::cout<<" ,cr down";

                        downTile = new Tile(field[y + 1][x]);
                        umap[10 * (y + 1) + x] = downTile;
                    }
                    else
                    {
                        // std::cout<<" ,con down";
                        downTile = umap[10 * (y + 1) + x];
                    }

                    currTile->tileBellow = downTile;
                }
                if (y >= 1 && field[y - 1][x] != 0)
                {
                    Tile *upTile;

                    if (umap.find(10 * (y - 1) + x) == umap.end())
                    {
                        // std::cout<<" ,cr up";

                        upTile = new Tile(field[y - 1][x]);
                        umap[10 * (y - 1) + x] = upTile;
                    }
                    else
                    {
                        // std::cout<<" ,con up";
                        upTile = umap[10 * (y - 1) + x];
                    }

                    currTile->tileAbove = upTile;
                }
                if(Utils::getTileFeature(field[y][x]) == "entrance"){
                    this->entrance = currTile;
                }
                // std::cout << Utils::getTileFeature(field[y][x])<<" " <<" "<<x<<","<<y<< "\n";
                if (Utils::getTileFeature(field[y][x]) == "escalator")
                {

                    int direction = Utils::getDirectionBitwise(field[y][x]);
                    // std::cout<<"diraciton"<<direction;
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

FieldPiece *FieldPiece::getLeftPiece()
{
    if (!this->leftPiece)
        throw std::invalid_argument("nullFieldPiece");
    return this->leftPiece;
}
FieldPiece *FieldPiece::getRightPiece()
{
    if (!this->rightPiece)
        throw std::invalid_argument("nullFieldPiece");
    return this->rightPiece;
}
FieldPiece *FieldPiece::getUpPiece()
{
    if (!this->upPiece)
        throw std::invalid_argument("nullFieldPiece");
    return this->upPiece;
}
FieldPiece *FieldPiece::getDownPiece()
{
    if (!this->downPiece)
        throw std::invalid_argument("nullFieldPiece");
    return this->downPiece;
}