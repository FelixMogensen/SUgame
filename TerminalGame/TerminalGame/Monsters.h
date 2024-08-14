#pragma once
#include <string>
#include <vector>

class Monster {
private:
    std::string name;
    int xp;
    int strength;
    int hp;
public:
    Monster(std::string name, int hp, int strength, int xp);

    std::string getName() const;
    int getHP() const;
    int getStrength() const;
    int getXP() const;

    void takeDamage(int damage);
    void displayInfo() const; 

    static std::vector<Monster> getMonsters();
};