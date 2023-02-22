#ifndef SQUARE_GAME_H
#define SQUARE_GAME_H

#include "square.h"
#include "player.h"
#include "game_type.h"

// Game playing Square (extends class Square).
// To add a new game you need to pass its cost, type and name.
class Square_game : public Square {
private:
    unsigned int game_cost;
    unsigned int money_stack = 0;
    game_type type;

public:
    explicit Square_game(std::string name, unsigned int cost, game_type t)
            : Square(std::move(name)), game_cost(cost), type(t) {};

    ~Square_game() override = default;

    void performMoveAction(Player &player) override {
        player.removeMoney(game_cost);
        money_stack += game_cost * type / 10;
    }
    void performStopAction(Player &player) override {
        player.takeMoney(money_stack);
        money_stack = 0;
    }
};

#endif //SQUARE_GAME_H