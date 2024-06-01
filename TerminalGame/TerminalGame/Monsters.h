#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <vector>

class Monster {
public:
    Monster(std::string name, int hp, int strength, int xp);

    std::string getName() const;
    int getHP() const;
    int getStrength() const;
    int getXP() const;

    void takeDamage(int damage);
    void displayInfo() const; 

    static std::vector<Monster> getMonsters();

private:
    std::string name;
    int hp;
    int strength;
    int xp;
};

#endif 
