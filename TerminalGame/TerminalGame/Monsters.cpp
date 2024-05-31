#include "Monsters.h"

Monster::Monster(std::string name, int hp, int strength, int xp)
    : name(name), hp(hp), strength(strength), xp(xp) {}

std::string Monster::getName() const {
    return name;
}

int Monster::getHP() const {
    return hp;
}

int Monster::getStrength() const {
    return strength;
}

int Monster::getXP() const {
    return xp;
}

void Monster::takeDamage(int damage) {
    hp -= damage;
}
