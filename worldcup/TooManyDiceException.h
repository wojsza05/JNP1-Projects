#ifndef TOO_MANY_DICE_EXCEPTION_H
#define TOO_MANY_DICE_EXCEPTION_H

#include <exception>

// Thrown if there are too many dices.
class TooManyDiceException : public  std::exception{
    [[nodiscard]] const char * what () const noexcept override {
        return "Too many dice";
    }
};


#endif //TOO_MANY_DICE_EXCEPTION_H
