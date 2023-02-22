#ifndef SQUARE_PENALTY_KICK_H
#define SQUARE_PENALTY_KICK_H

#include "square.h"
#include "player.h"
#include <tuple>

// Penalty Square (extends class Square).
class Square_penalty_kick : public Square {
private:
    unsigned int penalty_cost;

public:
    explicit Square_penalty_kick(std::string name, unsigned int cost)
            : Square(std::move(name)), penalty_cost(cost) {};

    ~Square_penalty_kick() override = default;

    void performMoveAction(Player &player) override {
        std::ignore = player;
    }

    void performStopAction(Player &player) override {
        player.removeMoney(penalty_cost);
    }
};

#endif //SQUARE_PENALTY_KICK_H
