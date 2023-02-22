#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include "square.h"

// Class that represents playing board that contains fields.
class Board {
private:
    std::vector<std::shared_ptr<Square>> squares;
public:
    Board() = default;
    ~Board() = default;
    void addPole(std::shared_ptr<Square> &&s) {
        squares.emplace_back(s);
    }
    Square* getPole(int i) {
        return squares[i].get();
    }
};


#endif //BOARD_H
