#ifndef SQUARE_GOAL_H
#define SQUARE_GOAL_H

#include "square.h"
#include "player.h"
#include <tuple>

// Goal Square (extends class Square).
class Square_goal : public Square {
private:
    unsigned int goal_bonus;

public:
    explicit Square_goal(std::string name, unsigned int bonus)
            : Square(std::move(name)), goal_bonus(bonus) {};

    ~Square_goal() override = default;

    void performMoveAction(Player &player) override {
        std::ignore = player;
    }

    void performStopAction(Player &player) override {
        player.takeMoney(goal_bonus);
    }
};

#endif //SQUARE_GOAL_H
