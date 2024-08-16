#pragma once
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
    int gold;
public:
    Hero(const std::string& n, int x = 0, int l = 1, int h = 10, int s = 2);

    std::string getName() const;
    int getXP() const;
    int getLevel() const;
    int getHP() const;
    int getStrength() const;
    int getGold() const;

    void displayInfo() const;
    void takeDamage(int damage);
    void gainXP(int experience);
    void levelUp();
    void addGold(int amount);

    void saveToDatabase(sqlite3* db);
    void deleteFromDatabase(sqlite3* db);
    static Hero loadFromDatabase(sqlite3* db, const std::string& heroName);
};