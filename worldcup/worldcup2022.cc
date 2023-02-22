#include "worldcup2022.h"
#include "player.h"
#include "square_beginning_of_the_season.h"
#include "square_goal.h"
#include "square_penalty_kick.h"
#include "square_bookmaker.h"
#include "square_yellow_card.h"
#include "square_game.h"
#include "square_no_action.h"
#include "TooManyDiceException.h"
#include "TooFewDiceException.h"
#include "TooFewPlayersException.h"
#include "TooManyPlayersException.h"

WorldCup2022::WorldCup2022() {
    board.addPole(std::make_shared<Square_beginning_of_the_season>("Początek sezonu"));
    board.addPole(std::make_shared<Square_game>("Mecz z San Marino", 160, friendly));
    board.addPole(std::make_shared<Square_no_action>("Dzień wolny od treningu"));
    board.addPole(std::make_shared<Square_game>("Mecz z Liechtensteinem", 220, friendly));
    board.addPole(std::make_shared<Square_yellow_card>("Żółta kartka", 3));
    board.addPole(std::make_shared<Square_game>("Mecz z Meksykiem", 300, points));
    board.addPole(std::make_shared<Square_game>("Mecz z Arabią Saudyjską", 280, points));
    board.addPole(std::make_shared<Square_bookmaker>("Bukmacher", 100, 100));
    board.addPole(std::make_shared<Square_game>("Mecz z Argentyną", 250, points));
    board.addPole(std::make_shared<Square_goal>("Gol", 120));
    board.addPole(std::make_shared<Square_game>("Mecz z Francją", 400, final));
    board.addPole(std::make_shared<Square_penalty_kick>("Rzut karny", 180));
}

void WorldCup2022::addPlayer(std::string const &name) {
    players.emplace_back(std::make_shared<Player>(name, 1000));
    num_of_players++;
    active_players++;
}

void WorldCup2022::setScoreBoard(std::shared_ptr<ScoreBoard> new_scoreboard) {
    this->scoreboard = new_scoreboard;
}

// Main function of the simulation.
void WorldCup2022::play(unsigned int rounds) {
    num_of_rounds = rounds;
    if (dice.getSize() < 2) {
        throw TooFewDiceException();
    }
    if (dice.getSize() > 2) {
        throw TooManyDiceException();
    }
    if (players.size() < 2) {
        throw TooFewPlayersException();
    }
    if (players.size() > 11) {
        throw TooManyPlayersException();
    }
    for (unsigned int i = 0; i < num_of_rounds; i++) {
        scoreboard->onRound(i);
        for (unsigned int j = 0; j < num_of_players; j++) {
            // j-player move.
            if (players[current_player]->isBankrupt()) {
                current_player = (current_player + 1) % num_of_players;
                continue;
            }
            if (!players[current_player]->isWaiting()) {
                int steps = performRound();
                int pos = players[current_player]->getCurrentPos();

                for (int k = 1; k < steps; k++) {
                    pos = (pos + 1) % 12;
                    if (!players[current_player]->isBankrupt())
                        board.getPole(pos)->performMoveAction(*players[current_player]);
                }

                pos = (pos + 1) % 12;
                if (!players[current_player]->isBankrupt())
                    board.getPole(pos)->performStopAction(*players[current_player]);

                players[current_player]->setCurrentPos(pos);
            }

            // Actualizing the scoreboard.
            scoreboard->onTurn(players[current_player]->getName(),
                               players[current_player]->getPlayerStatus(),
                               board.getPole(players[current_player]->
                                       getCurrentPos())->getName(),
                               players[current_player]->getMoney());
            // Checking if player has gone bankrupt.
            if (players[current_player]->isBankrupt()) {
                active_players--;
                if (active_players == 1) // Game over.
                    for (unsigned int k = 0; k < num_of_players; k++)
                        if (!players[k]->isBankrupt()) {
                            scoreboard->onWin(players[k]->getName());
                            return;
                        }
            }

            if (players[current_player]->isWaiting())
                players[current_player]->tick();

            current_player = (current_player + 1) % num_of_players;
        }
    }
}

int WorldCup2022::performRound() {
    return dice.performRound();
}

void WorldCup2022::addDie(std::shared_ptr<Die> die) {
    dice.addDie(die);
}
