#include "player.h"

#include <utility>

Player::Player(std::string name, unsigned int money) :
        name(std::move(name)), money(money) {};

// getters
std::string Player::getName() { return name; }
unsigned int Player::getMoney() const { return money; }

// methode
void Player::removeMoney(unsigned int  amount) {
    if (money < amount) {
        bankrupt = true;
        money = 0;
    } else {
        money -= amount;
    }
}

void Player::takeMoney(unsigned int amount) {
    money += amount;
}

// other methods
void Player::waitTurns(int turns) {
    waitingTurns += turns;
}

bool Player::isBankrupt() const {
    return bankrupt;
}

bool Player::isWaiting() const {
    return waitingTurns > 0;
}

void Player::tick() {
    if (waitingTurns > 0)
        --waitingTurns;
}

int Player::getCurrentPos() const {
    return currentPos;
}

void Player::setCurrentPos(int newPos) {
    currentPos = newPos;
}

std::string Player::getPlayerStatus() const {
    if (isBankrupt()) {
        return "*** bankrut ***";
    } else if (isWaiting()) {
        std::string numberOfTurns = std::to_string(waitingTurns);
        return "*** czekanie: " + numberOfTurns + " ***";
    } else {
        return "w grze";
    }
}
