#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

class Monster {
private:
    std::string name;
    int xp;
    int strength;
    int hp;
    int originalhp;
public:
    Monster() = default;
    Monster(std::string name, int hp, int strength, int xp);

    std::string getName() const;
    int getHP() const;
    int getStrength() const;
    int getXP() const;

    void takeDamage(int damage);
    void respawn();
    void displayInfo() const; 

    static std::vector<Monster> getMonsters(sqlite3* db);

    void saveToDatabase(sqlite3* db, const std::string& heroName);
    static std::vector<Monster> loadAllFromDatabase(sqlite3* db, const std::string& heroName);
    static void deleteMonstersForHero(sqlite3* db, const std::string& heroName);
};