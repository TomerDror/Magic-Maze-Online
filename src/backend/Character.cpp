
#include "Character.h"

Character::Character(std::string name, FieldPiece *startingFieldPiece, Tile *startingTile)
{
    this->name = name;
    this->fieldPieceOn = startingFieldPiece;
    this->tileOn = startingTile;
}

std::vector<Tile *> Character::getPlausibleTargetTiles(MovementAbility *playerMovementAbility)
{
    std::vector<Tile *> plausibleTargetTiles;
    if (this->tileOn != nullptr)
    {
        plausibleTargetTiles = (tileOn)->getPlausibleTargetTiles(playerMovementAbility);
    }

    std::vector<Tile *> portals;
    if (this->name == "green")
        portals = Field::getInstance()->greenPortals;
    if (this->name == "purple")
        portals = Field::getInstance()->purplePortals;
    if (this->name == "orange")
        portals = Field::getInstance()->orangePortals;
    if (this->name == "yellow")
        portals = Field::getInstance()->yellowPortals;
    for (Tile *tile : portals)
    {
        // if()
        if (tile != nullptr && playerMovementAbility->canUsePortals && Field::getInstance()->isTileVacated(tile))
        {
            plausibleTargetTiles.push_back(tile);
        }
        //
    }
    return plausibleTargetTiles;
}

// void Character::move(Tile *tile,MovementAbility *playerMovementAbility){
//     std::vector<Tile*> plausibleTiles = getPlausibleTargetTiles(playerMovementAbility);
//     if(std::find(plausibleTiles.begin(), plausibleTiles.end(), tile)!= plausibleTiles.end()){
//         this->tileOn = tile;
//     }
// }

void Character::move(Tile *tile, MovementAbility *playerMovementAbility)
{
    // Check if the tile is a plausible target tile
    std::vector<Tile *> plausibleTargetTiles = getPlausibleTargetTiles(playerMovementAbility);
    bool isValidMove = false;
    for (Tile *targetTile : plausibleTargetTiles)
    {
        if (targetTile == tile)
        {
            isValidMove = true;
            break;
        }
    }
    if (isValidMove)
    {
        this->tileOn = tile;
    }
}
void Character::openFieldPiece()
{ // TODO
  //  if (Utils::getTileFeature(this->tileOn->tileType) == "opening" && this->name == Utils::getTileColor(this->tileOn->tileType)  && isFieldPieceAlreadyExist(Utils::getDirection(this->tileOn->tileType), this->tileOn->fieldPieceOn ))//this->tileOn->fieldPieceOn->upPiece) // add check that there isnt a fp in that direction TODo
  //  {

    if (!Field::getInstance()->futureFieldPieces.empty())
    {
        FieldPiece *newFieldPiece;
        std::string direction = Utils::getDirection(this->tileOn->tileType);
        // std::cout << direction;
        int x = this->tileOn->fieldPieceOn->x;
        int y = this->tileOn->fieldPieceOn->y;
        std::cout<<"work";
        PreFieldPiece preFieldPiece = *(Field::getInstance()->allFieldPieces[Field::getInstance()->futureFieldPieces.front()]);
        if (direction == "up")
        {
            newFieldPiece = new FieldPiece(x, y + 1, Field::getInstance(), &preFieldPiece);
            Field::getInstance()->futureFieldPieces.pop();
            Field::getInstance()->openedFieldPieces.push_back(newFieldPiece);
        }
        else if (direction == "down")
        {
            preFieldPiece.rotateLeft();
            preFieldPiece.rotateLeft();

            newFieldPiece = new FieldPiece(x, y - 1, Field::getInstance(), &preFieldPiece);
            Field::getInstance()->futureFieldPieces.pop();
            Field::getInstance()->openedFieldPieces.push_back(newFieldPiece);
        }
        else if (direction == "right")
        {
            preFieldPiece.rotateRight();

            newFieldPiece = new FieldPiece(x + 1, y, Field::getInstance(), &preFieldPiece);
            Field::getInstance()->futureFieldPieces.pop();
            Field::getInstance()->openedFieldPieces.push_back(newFieldPiece);
        }
        else if (direction == "left")
        {
            std::cout<<"heyy";
            preFieldPiece.rotateLeft();
            std::cout<<"reg";

            newFieldPiece = new FieldPiece(x - 1, y, Field::getInstance(), &preFieldPiece);
            Field::getInstance()->futureFieldPieces.pop();
            Field::getInstance()->openedFieldPieces.push_back(newFieldPiece);
        }
        else
        {

            std::cout << "invalid direction";
            return;
        }
        connectAdjacentFieldPieces(newFieldPiece);
        // connect fp
        // connect tiles

        // TODO rotate fp

        // change tile type
    }
    // }
}
bool Character::isFieldPieceAlreadyExist(std::string direction, FieldPiece *fieldPiece)
{
    if (direction == "up")
    {
        return fieldPiece->upPiece != nullptr;
    }
    else if (direction == "down")
    {
        return fieldPiece->downPiece != nullptr;
    }
    else if (direction == "right")
    {
        return fieldPiece->rightPiece != nullptr;
    }
    else if (direction == "left")
    {
        return fieldPiece->leftPiece != nullptr;
    }

    std::cout << "invalid direction";
    return false;
}

void Character::connectAdjacentFieldPieces(FieldPiece *newFieldPiece)
{
    for (FieldPiece *fieldPiece : Field::getInstance()->openedFieldPieces)
    {
        if (newFieldPiece->x == fieldPiece->x && newFieldPiece->y == fieldPiece->y + 1)
        { // opening above

            for (Tile *openingInNewFieldPiece : newFieldPiece->openings)
            {
                for (Tile *openingInFieldPiece : fieldPiece->openings)
                {
                    // std::cout << " new tile " << Utils::getDirection(openingInNewFieldPiece->tileType) << " old tile " << Utils::getDirection(openingInFieldPiece->tileType) << "\n";
                    if (Utils::getDirection(openingInNewFieldPiece->tileType) == "down" && Utils::getDirection(openingInFieldPiece->tileType) == "up")
                    {
                        // std::cout << "KA KA";
                        newFieldPiece->downPiece = fieldPiece;
                        fieldPiece->upPiece = newFieldPiece;
                        openingInNewFieldPiece->tileBellow = openingInFieldPiece;
                        openingInFieldPiece->tileAbove = openingInNewFieldPiece;
                    }
                }
            }
        }
        if (newFieldPiece->x == fieldPiece->x && newFieldPiece->y == fieldPiece->y - 1)
        { // opening bellow
            for (Tile *openingInNewFieldPiece : newFieldPiece->openings)
            {
                for (Tile *openingInFieldPiece : fieldPiece->openings)
                {
                    // std::cout << " new tile " << Utils::getDirection(openingInNewFieldPiece->tileType) << " old tile " << Utils::getDirection(openingInFieldPiece->tileType) << "\n";
                    if (Utils::getDirection(openingInNewFieldPiece->tileType) == "up" && Utils::getDirection(openingInFieldPiece->tileType) == "down")
                    {
                        newFieldPiece->upPiece = fieldPiece;
                        fieldPiece->downPiece = newFieldPiece;
                        openingInNewFieldPiece->tileAbove = openingInFieldPiece;
                        openingInFieldPiece->tileBellow = openingInNewFieldPiece;
                    }
                }
            }
        }
        if (newFieldPiece->x == fieldPiece->x + 1 && newFieldPiece->y == fieldPiece->y)
        { // opening to right

            for (Tile *openingInNewFieldPiece : newFieldPiece->openings)
            {
                for (Tile *openingInFieldPiece : fieldPiece->openings)
                {
                    // std::cout << "right new tile " << Utils::getDirection(openingInNewFieldPiece->tileType) << " old tile " << Utils::getDirection(openingInFieldPiece->tileType) << "\n";
                    if (Utils::getDirection(openingInNewFieldPiece->tileType) == "left" && Utils::getDirection(openingInFieldPiece->tileType) == "right")
                    {
                        // std::cout << "KA KA" << openingInNewFieldPiece->tileType / 1000000 << openingInFieldPiece->tileType / 1000000;
                        newFieldPiece->leftPiece = fieldPiece;
                        fieldPiece->rightPiece = newFieldPiece;
                        openingInNewFieldPiece->tileToLeft = openingInFieldPiece;
                        openingInFieldPiece->tileToRight = openingInNewFieldPiece;
                    }
                }
            }
        }
        if (newFieldPiece->x == fieldPiece->x - 1 && newFieldPiece->y == fieldPiece->y)
        {
            for (Tile *openingInNewFieldPiece : newFieldPiece->openings)
            {
                for (Tile *openingInFieldPiece : fieldPiece->openings)
                {
                    // std::cout << "left new tile " << Utils::getDirection(openingInNewFieldPiece->tileType) << " old tile " << Utils::getDirection(openingInFieldPiece->tileType) << "\n";
                    if (Utils::getDirection(openingInNewFieldPiece->tileType) == "right" && Utils::getDirection(openingInFieldPiece->tileType) == "left")
                    {
                        // std::cout << "KA KA" << openingInNewFieldPiece->tileType / 1000000 <<" "<< openingInFieldPiece->tileType / 1000000<<" ";
                        // std::cout <<  openingInNewFieldPiece->tileType<<" right "  << openingInFieldPiece->tileType <<"\n";
                        newFieldPiece->rightPiece = fieldPiece;
                        fieldPiece->leftPiece = newFieldPiece;
                        openingInNewFieldPiece->tileToRight = openingInFieldPiece;
                        openingInFieldPiece->tileToLeft = openingInNewFieldPiece;
                    }
                }
            }
        }
    }
}