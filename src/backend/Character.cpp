
#include "Character.h"

// class Character
// {
// public:
//     std::string name;
//     FieldPiece *fieldPieceOn;
//     Tile *tileOn;
//     Character(std::string, FieldPiece *startingPieceField, Tile *startingTile);
//     std::vector<Tile*> getPlausibleTargetTiles(MovementAbility *playerMovementAbility);
// };

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
        portals = Field::getInstance().greenPortals;
    if (this->name == "purple")
        portals = Field::getInstance().purplePortals;
    if (this->name == "orange")
        portals = Field::getInstance().orangePortals;
    if (this->name == "yellow")
        portals = Field::getInstance().yellowPortals;
    for (Tile *tile : portals)
    {
        // std::cout<<"fasdf";
        // if()
        if (tile != nullptr && playerMovementAbility->canUsePortals && Field::getInstance().isTileVacated(tile))
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
    // std::cout<<"\n"<< isValidMove<<"\n";
    if (isValidMove)
    {
        this->tileOn = tile;
    }
}
void Character::openFieldPiece()
{
    if (Utils::getTileFeature(this->tileOn->tileType) == "opening" && this->name == Utils::getTileColor(this->tileOn->tileType)  && isFieldPieceAlreadyExist(Utils::getDirection(this->tileOn->tileType), this->tileOn->fieldPieceOn ))//this->tileOn->fieldPieceOn->upPiece) // add check that there isnt a fp in that direction TODo
    {
        if (!Field::getInstance().futureFieldPieces.empty())
        {
            FieldPiece *newFieldPiece;
            std::string direction = Utils::getDirection(this->tileOn->tileType);
            if (direction == "up")
            {
                PreFieldPiece preFieldPiece = Field::getInstance().allFieldPieces[Field::getInstance().futureFieldPieces.front()];
                newFieldPiece = new FieldPiece(&preFieldPiece);
                Field::getInstance().futureFieldPieces.pop();
                
                this->tileOn->fieldPieceOn->upPiece = newFieldPiece;
                newFieldPiece->downPiece = this->tileOn->fieldPieceOn;

                this->tileOn->tileAbove = newFieldPiece->entrance;
                newFieldPiece->entrance->tileBellow = this->tileOn;
            }
            else if (direction == "down")
            {
                PreFieldPiece preFieldPiece = Field::getInstance().allFieldPieces[Field::getInstance().futureFieldPieces.front()];
                preFieldPiece.rotateLeft();
                preFieldPiece.rotateLeft();
                newFieldPiece = new FieldPiece(&preFieldPiece);
                Field::getInstance().futureFieldPieces.pop();

                this->tileOn->fieldPieceOn->downPiece = newFieldPiece;
                newFieldPiece->upPiece = this->tileOn->fieldPieceOn;
                this->tileOn->tileBellow = newFieldPiece->entrance;
                newFieldPiece->entrance->tileAbove = this->tileOn;
            }
            else if (direction == "right")
            {
                PreFieldPiece preFieldPiece = Field::getInstance().allFieldPieces[Field::getInstance().futureFieldPieces.front()];
                preFieldPiece.rotateRight();
                newFieldPiece = new FieldPiece(&preFieldPiece);
                Field::getInstance().futureFieldPieces.pop();

                this->tileOn->fieldPieceOn->rightPiece = newFieldPiece;
                newFieldPiece->leftPiece = this->tileOn->fieldPieceOn;
                this->tileOn->tileToRight = newFieldPiece->entrance;
                newFieldPiece->entrance->tileToLeft = this->tileOn;
            }
            else if (direction == "left")
            {
                PreFieldPiece preFieldPiece = Field::getInstance().allFieldPieces[Field::getInstance().futureFieldPieces.front()];
                preFieldPiece.rotateLeft();
                newFieldPiece = new FieldPiece(&preFieldPiece);
                Field::getInstance().futureFieldPieces.pop();
                
                this->tileOn->fieldPieceOn->leftPiece = newFieldPiece;
                newFieldPiece->rightPiece = this->tileOn->fieldPieceOn;
                this->tileOn->tileToLeft = newFieldPiece->entrance;
                newFieldPiece->entrance->tileToRight = this->tileOn;
            }
            else
            {
                std::cout << "invalid direction";
            }
            // connect fp
            // connect tiles

            // TODO rotate fp

            // change tile type
            Utils::setTileFeature(&this->tileOn->tileType, 1);
        }
    }
}
bool Character::isFieldPieceAlreadyExist(std::string direction, FieldPiece *fieldPiece){
            if (direction == "up")
            {
                return fieldPiece->upPiece !=nullptr;
                
            }
            else if (direction == "down")
            {
                return fieldPiece->downPiece !=nullptr;
                
            }
            else if (direction == "right")
            {
                return fieldPiece->rightPiece !=nullptr;
                
            }
            else if (direction == "left")
            {
                return fieldPiece->leftPiece !=nullptr;
                
            }
           
                std::cout << "invalid direction";
                return false;
            
}