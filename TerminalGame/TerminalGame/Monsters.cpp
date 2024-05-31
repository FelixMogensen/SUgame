#include <iostream>
#include <string>
#include "Monsters.h"
#include <vector>

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
    if (hp < 0) hp = 0;
}

std::vector<Monster> Monster::getPredefinedMonsters() {
    return {
        Monster("Goblin", 4, 2, 200),
        Monster("Orc", 8, 3, 400),
        Monster("Dragon", 100, 10, 3000)
    };
}

void Monster::displayInfo() const {
    std::cout << "Monster " << name << " with remaning HP: " << hp << "\n";
    
}
