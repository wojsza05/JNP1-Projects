#ifndef PLAYER_H
#define PLAYER_H

#include <string>

// Player class
class Player {
private:
    std::string name;
    int currentPos = 0;
    unsigned int money;
    int waitingTurns = 0;
    bool bankrupt = false;

public:
    explicit Player(std::string name, unsigned int money);

    [[nodiscard]] std::string getPlayerStatus() const;

    void removeMoney(unsigned int amount);

    void takeMoney(unsigned int amount);

    [[nodiscard]] unsigned int getMoney() const;

    std::string getName();

    [[nodiscard]] bool isBankrupt() const;

    [[nodiscard]] int getCurrentPos() const;

    void setCurrentPos(int newPos);

    void tick();

    [[nodiscard]] bool isWaiting() const;

    void waitTurns(int turns);
};

#endif //PLAYER_H
