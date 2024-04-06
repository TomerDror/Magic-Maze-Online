#include "Field.h"
#include "Character.h"
#include "PreFieldPiece.h"
#include "FieldPiece.h"
#include "Player.h"
#include "Tile.h"
#include <iostream>
#include <algorithm>

void printPossibleMoves(MovementAbility *movementAbility);
void printPossibleMoves(Character *character, MovementAbility *movementAbility);

int main()
{
 
    // int size = 4;
    //     int** tiles = new int*[size];
    //     for (int i = 0; i < size; ++i)
    //     {
    //         tiles[i] = new int[size];
    //         for (int j = 0; j < size; ++j)
    //         {
    //             tiles[i][j] = 4*i+j;
    //         }
    //     }
    //     PreFieldPiece preFieldPiece(tiles, size);
    //     FieldPiece FieldPiece

    // Field& fieldInstance = Field::getInstance();
    Player firstPlayer(1, 1, 0, 1, 1, 1, 1);

    while (true)
    {
        printPossibleMoves(firstPlayer.movementAbility);
        std::string cmd;
        std::string color;
        int number = 0;

        std::cout << "Enter a command: ";
        std::cin >> cmd;
        cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end());

        if (cmd == "stop")
        {
            break;
        }
        if (cmd == "move")
        {
            std::cout << "Enter a color: ";
            std::cin >> color;

            while (true)
            {
                std::cout << "Enter a number: ";
                if (!(std::cin >> number))
                {
                    std::cout << "Please enter a valid number." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(100, '\n');
                }
                else
                {
                    break;
                }
            }

            color.erase(std::remove(color.begin(), color.end(), ' '), color.end());
            if (color == "green")
            {
                std::vector<Tile *> possibleTiles = Field::getInstance().getGreenCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
                for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
                {
                    if (possibleTiles[i]->tileType == number)
                    {
                        Field::getInstance().getGreenCharacter()->move(possibleTiles[i], firstPlayer.movementAbility);
                    }
                }
            }
            if (color == "purple")
            {
                std::vector<Tile *> possibleTiles = Field::getInstance().getPurpleCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
                for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
                {
                    if (possibleTiles[i]->tileType == number)
                    {
                        Field::getInstance().getPurpleCharacter()->move(possibleTiles[i], firstPlayer.movementAbility);
                    }
                }
            }
            if (color == "orange")
            {
                std::vector<Tile *> possibleTiles = Field::getInstance().getOrangeCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
                for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
                {
                    if (possibleTiles[i]->tileType == number)
                    {
                        Field::getInstance().getOrangeCharacter()->move(possibleTiles[i], firstPlayer.movementAbility);
                    }
                }
            }
            if (color == "yellow")
            {
                std::vector<Tile *> possibleTiles = Field::getInstance().getYellowCharacter()->getPlausibleTargetTiles(firstPlayer.movementAbility);
                for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
                {
                    if (possibleTiles[i]->tileType == number)
                    {
                        Field::getInstance().getYellowCharacter()->move(possibleTiles[i], firstPlayer.movementAbility);
                    }
                }
            }
        }
    } // move green 102

    return 0;
}

void printPossibleMoves(MovementAbility *movementAbility)
{
    printPossibleMoves(Field::getInstance().getGreenCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getPurpleCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getYellowCharacter(), movementAbility);
    printPossibleMoves(Field::getInstance().getOrangeCharacter(), movementAbility);
    // printPossibleMoves(getInstance().getGreenCharacter(),movementAbility);
}

void printPossibleMoves(Character *character, MovementAbility *movementAbility)
{
    std::cout << "possible " << (character->name) << " tiles ";
    std::vector<Tile *> possibleTiles = character->getPlausibleTargetTiles(movementAbility);
    for (std::vector<Tile *>::size_type i = 0; i < possibleTiles.size(); ++i)
    {

        std::cout << possibleTiles[i]->tileType << " ";
    }
    std::cout << "\n";
}