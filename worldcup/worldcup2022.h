#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include "square.h"
#include "player.h"
#include "board.h"
#include "dices.h"
#include "worldcup.h"
#include <vector>
#include <string>
#include <map>

// Zalozenia obiektowe:
// Klasa Worldcup2022 implementuje interfejs Worldcup i zawiera atrybuty,
// które są własnymi obiekatami.
class WorldCup2022: public WorldCup {
private:
    Dices dice;
    std::vector<std::shared_ptr<Player>> players;
    Board board;
    std::shared_ptr<ScoreBoard> scoreboard;
    unsigned int current_player = 0;
    unsigned int num_of_rounds = 0;
    unsigned int num_of_players = 0;
    unsigned int active_players = 0;
    int performRound();
public:
    WorldCup2022();
    void addDie(std::shared_ptr<Die> die) override;
    void addPlayer(std::string const &name) override;
    void setScoreBoard(std::shared_ptr<ScoreBoard> scoreboard) override;
    void play(unsigned int rounds) override;
};

#endif