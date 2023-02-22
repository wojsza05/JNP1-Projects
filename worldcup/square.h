#ifndef SQUARE_H
#define SQUARE_H

#include "player.h"
#include <string>
#include <utility>

// Virtual Class that represents a field on a playing board.
class Square {
protected:
    std::string name;

public:
    explicit Square(std::string name) : name(std::move(name)) {};

    virtual ~Square() = default;

    virtual void performMoveAction(Player &player) = 0;

    virtual void performStopAction(Player &player) = 0;

    [[nodiscard]] std::string getName() const {
        return name;
    }
};

#endif //SQUARE_H
