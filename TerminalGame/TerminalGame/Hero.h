#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <string>
#include <sqlite3.h>

class Hero {
private:
    std::string name;
    int xp;
    int level;
    int hp;
    int strength;
    bool defeated;

    void levelUp();

public:
    Hero(const std::string& n, int x = 0, int l = 1, int h = 10, int s = 2);

    std::string getName() const;
    int getXP() const;
    int getLevel() const;
    int getHP() const;
    int getStrength() const;
    bool isDefeated() const;

    void setName(const std::string& n);
    void setXP(int x);
    void setLevel(int l);
    void setHP(int h);
    void setStrength(int s);

    void displayInfo() const;
    void takeDamage(int damage);
    void gainXP(int experience);
    void saveToDatabase(sqlite3* db);

    void deleteFromDatabase(sqlite3* db);

    static Hero loadFromDatabase(sqlite3* db, const std::string& heroName);
};

#endif // HERO_H
