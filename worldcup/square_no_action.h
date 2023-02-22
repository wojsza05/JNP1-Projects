#ifndef SQUARE_NO_ACTION_H
#define SQUARE_NO_ACTION_H

#include "square.h"
#include <tuple>

// Starting Square (extends class Square).
class Square_no_action : public Square {
public:
    explicit Square_no_action(std::string name)
        : Square(std::move(name)) {};

    void performMoveAction(Player &player) override {
        std::ignore = player;
    }

    void performStopAction(Player &player) override {
        std::ignore = player;
    }
};

#endif //SQUARE_NO_ACTION_H
