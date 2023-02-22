#ifndef SQUARE_YELLOW_CARD_H
#define SQUARE_YELLOW_CARD_H

#include "square.h"
#include "player.h"
#include <tuple>

// Yellow card Square (extends class Square).
class Square_yellow_card : public Square {
private:
    int number_of_turns;

public:
    explicit Square_yellow_card(std::string name, int number_of_turns)
        : Square(std::move(name)), number_of_turns(number_of_turns) {};

    ~Square_yellow_card() override = default;

    void performMoveAction(Player &player) override {
        std::ignore = player;
    }

    void performStopAction(Player &player) override {
        player.waitTurns(number_of_turns);
    }
};

#endif //SQUARE_YELLOW_CARD_H
