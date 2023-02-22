#ifndef TOO_FEW_DICE_EXCEPTION_H
#define TOO_FEW_DICE_EXCEPTION_H

#include <exception>

// Thrown if there are too few dices.
class TooFewDiceException : public std::exception{
    [[nodiscard]] const char * what () const noexcept override {
        return "Too few dice";
    }
};


#endif //TOO_FEW_DICE_EXCEPTION_H
