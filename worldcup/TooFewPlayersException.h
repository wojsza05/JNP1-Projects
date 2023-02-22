#ifndef TOO_FEW_PLAYERS_EXCEPTION_H
#define TOO_FEW_PLAYERS_EXCEPTION_H


#include <exception>

// Thrown if there are too few players.
class TooFewPlayersException : public std::exception{
    [[nodiscard]] const char * what () const noexcept override {
        return "Too few players";
    }
};


#endif //TOO_FEW_PLAYERS_EXCEPTION_H
