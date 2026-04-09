// Player.cpp - Player implementation
#include "Player.h"
#include "Qt"

Player::Player(PlayerType t, InputHandler* input)
    : Character(100, 0, 5, 10), // hp, x, speed, attackPower
    score(0),
    type(t),
    input(input)
{
    setStats();
}

void Player::setStats() {
    switch (type) {
    case TANK:
        maxHp = 150;
        hp = 150;
        speed = 3;
        attackPower = 8;
        break;

    case FAST:
        maxHp = 80;
        hp = 80;
        speed = 7;
        attackPower = 6;
        break;

    case BALANCED:
        maxHp = 100;
        hp = 100;
        speed = 5;
        attackPower = 10;
        break;
    }
}

void Player::move() {
    if (!input) return;

    if (input->isKeyPressed(Qt::Key_Left)) {
        x -= speed;
    }

    if (input->isKeyPressed(Qt::Key_Right)) {
        x += speed;
    }
}

void Player::attack(Character* target) {
    if (target && target->isAlive()) {
        target->takeDamage(attackPower);
    }
}

void Player::addScore(int s) {
    score += s;
}

int Player::getScore() const {
    return score;
}