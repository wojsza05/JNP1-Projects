#ifndef SQUARE_BEGINNING_OF_THE_SEASON_H
#define SQUARE_BEGINNING_OF_THE_SEASON_H

#include "square.h"

// Certain type of a square (extends class Square).
class Square_beginning_of_the_season : public Square {
public:
    explicit Square_beginning_of_the_season(std::string name)
        : Square(std::move(name)) {};

    void performMoveAction(Player &player) override {
        player.takeMoney(50);
    }
    void performStopAction(Player &player) override {
        performMoveAction(player);
    }
};

#endif //SQUARE_BEGINNING_OF_THE_SEASON_H