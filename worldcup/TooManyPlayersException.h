#ifndef TOO_MANY_PLAYERS_EXCEPTION_H
#define TOO_MANY_PLAYERS_EXCEPTION_H

#include <exception>

// Thrown if there are too many players.
class TooManyPlayersException : public std::exception{
    [[nodiscard]] const char * what () const noexcept override {
        return "Too many players";
    }
};


#endif //TOO_MANY_PLAYERS_EXCEPTION_H
