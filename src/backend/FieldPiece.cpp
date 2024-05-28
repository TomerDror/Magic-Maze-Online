

#include "FieldPiece.h"
FieldPiece::FieldPiece(int x, int y, Field *playingField, PreFieldPiece *preFieldPiece)
{
    this->x = x;
    this->y = y;
    // std::cout << "x " << this->x << ", y " << this->y;
    int size = preFieldPiece->getSize();
    int **field = preFieldPiece->getData();
    std::unordered_map<int, Tile *> umap;
    bool isFirst = true;
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            // std::cout<<x<<", "<<y <<" "<<field[y][x] <<"\n";
            if (field[y][x] != 0)
            {
                Tile *currTile;

                if (umap.find(10 * y + x) == umap.end())
                {
                    // std::cout<<" creating tile ";

                    currTile = new Tile(this, field[y][x]);
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
                if (Utils::getTileFeature(currTile->tileType) == "opening" || Utils::getTileFeature(currTile->tileType) == "entrance")
                {
                    this->openings.push_back(currTile);
                    // std::cout << "\n openingss " << openings.size();
                }
                // std::cout << "\nright\n";
                this->locateTile(&(currTile->tileToRight), x + 1, y, field, size, &umap, Utils::tileBlockedMoveRight(currTile->tileType));
                // std::cout << "left\n";
                this->locateTile(&(currTile->tileToLeft), x - 1, y, field, size, &umap, Utils::tileBlockedMoveLeft(currTile->tileType));
                // std::cout << "down\n";
                this->locateTile(&(currTile->tileBellow), x, y + 1, field, size, &umap, Utils::tileBlockedMoveDown(currTile->tileType));
                // std::cout << "up\n";
                this->locateTile(&(currTile->tileAbove), x, y - 1, field, size, &umap, Utils::tileBlockedMoveUp(currTile->tileType));

                if (Utils::getTileFeature(field[y][x]) == "entrance")
                {
                    this->entrance = currTile;
                }
                // std::cout << Utils::getTileFeature(field[y][x])<<" " <<" "<<x<<","<<y<< "\n";

                if (Utils::getEscalatorDirectionBitwise(field[y][x]) != 0)
                {

                    int direction = Utils::getEscalatorDirectionBitwise(field[y][x]);
                    // std::cout << " escalator : " << direction << " " << field[y][x] ;
                    int escalatorX = x;
                    int escalatorY = y;
                    switch (direction)
                    {
                    case 1: // clock 1
                        escalatorX += 1;
                        escalatorY -= 2;
                        break;
                    case 2: // clock 2
                        escalatorX += 2;
                        escalatorY -= 1;
                        break;
                    case 4: // clock 4
                        escalatorX += 2;
                        escalatorY += 1;
                        break;
                    case 5: // clock 5
                        escalatorX += 1;
                        escalatorY += 2;
                        break;
                    case 7: // clock 7
                        escalatorX -= 1;
                        escalatorY += 2;
                        break;
                    case 8: // clock 8
                        escalatorX -= 2;
                        escalatorY += 1;
                        break;
                    case 10: // clock 10
                        escalatorX -= 2;
                        escalatorY -= 1;
                        break;
                    case 11: // clock 11
                        escalatorX -= 1;
                        escalatorY -= 2;
                        break;
                    case 12: // up right 12
                        escalatorX += 1;
                        escalatorY -= 1;
                        break;
                    case 13: // down right 13
                        escalatorX += 1;
                        escalatorY += 1;
                        break;
                    case 14: // down left 14
                        escalatorX -= 1;
                        escalatorY += 1;
                        break;
                    case 15: // downup right 15
                        escalatorX -= 1;
                        escalatorY -= 1;
                        break;

                    default:
                        std::cout << "escalator error: ";
                        break;
                    }

            // std::cout<<" tjt";
                    Tile *escalatorTile;
                    // std::cout<<"esc x,y "<< escalatorX<<","<<escalatorY<<"/n";
                    if (umap.find(10 * (escalatorY) + escalatorX) == umap.end())
                    {

                        // std::cout<<"not ok ok";
                        escalatorTile = new Tile(this, field[escalatorY][escalatorX]);
                        umap[10 * (escalatorY) + escalatorX] = escalatorTile;
                    }
                    else
                    {
                        // std::cout<<"ok ok";
                        // std::cout << "\n3\n";
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
    // std::cout<<"x "<<this->x<<", y "<<this->y;
    // std::cout << "openings length " << this->openings.size() << "\n";
}
void FieldPiece::locateTile(Tile **returnTile, int x, int y, int **field, int size, std::unordered_map<int, Tile *> *umap_ptr, bool blockedInDirection)
{
    // if (x >= 0 && y >= 0 && x<=4 &&y<=4)
        // std::cout<<"asfadf"<<x <<" "<<y;
        // std::cout << " blocked in direction " <<!blockedInDirection;
    if (x < size && x >= 0 && y < size && y >= 0 && field[y][x] != 0 && !blockedInDirection)
    {
        // std::cout << " "< x << ", " << y <<" "<<(*returnTile)->tileType<< "   " << field[y][x] << "\n";
        Tile *tileInDirection;
        if ((*umap_ptr).find(10 * (y) + x) == (*umap_ptr).end())
        {
            // std::cout<<" ,cr up";

            tileInDirection = new Tile(this, field[y][x]);
            (*umap_ptr)[10 * (y) + x] = tileInDirection;
        }
        else
        {
            // std::cout<<" ,con up";
            tileInDirection = (*umap_ptr)[10 * (y) + x];
        }
        // std::cout<<x<<", "<< y<< " "<< tileInDirection->tileType <<"\n";
        (*returnTile) = tileInDirection;
    }
    else
    {
        (*returnTile) = nullptr;
    }
}