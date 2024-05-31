#ifndef MONSTER_H
#define MONSTER_H

#include <string>

class Monster {
public:
    Monster(std::string name, int hp, int strength, int xp);

    std::string getName() const;
    int getHP() const;
    int getStrength() const;
    int getXP() const;

    void takeDamage(int damage);

private:
    std::string name;
    int hp;
    int strength;
    int xp;
};

#endif // MONSTER_H
