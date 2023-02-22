#ifndef SQUARE_BOOKMAKER_H
#define SQUARE_BOOKMAKER_H

#include "square.h"
#include "player.h"
#include <tuple>

// Bookmaker square (extends class Square).
class Square_bookmaker : public Square {
private:
    unsigned int bookmaker_bonus;
    unsigned int bookmaker_loss;
    unsigned int count_players = 0;

public:
    explicit Square_bookmaker(std::string name, unsigned int bonus, unsigned int loss)
            : Square(std::move(name)), bookmaker_bonus(bonus),
            bookmaker_loss(loss) {};

    ~Square_bookmaker() override = default;

    void performMoveAction(Player &player) override {
        std::ignore = player;
    }

    void performStopAction(Player &player) override {
        if (count_players == 0)
            player.takeMoney(bookmaker_bonus);
        else
            player.removeMoney(bookmaker_loss);

        count_players = (count_players + 1) % 3;
    }
};

#endif //SQUARE_BOOKMAKER_H
