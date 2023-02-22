#ifndef DICES_H
#define DICES_H

#include "worldcup.h"
#include <vector>

class Dices {
private:
    std::vector<std::shared_ptr<Die>> dice;
public:
    Dices() = default;
    ~Dices() = default;

    void addDie(std::shared_ptr<Die> die) {
        if (die != nullptr)
        dice.push_back(die);
    }

    int performRound() {
        int roll1 = dice[0]->roll();
        int roll2 = dice[1]->roll();
        return roll1 + roll2;
    }

    int getSize() {
        return (int)dice.size();
    }
};


#endif //DICES_H
