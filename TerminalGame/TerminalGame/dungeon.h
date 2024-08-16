#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Monsters.h"
#include <sqlite3.h>
#include <unordered_set>

class Dungeon {
private:
    std::string name;
    int gold;
    std::vector<Monster> monsters;
    std::unordered_set<std::string> defeatedMonsters;
    bool completed = false;
public:
    Dungeon(std::string name, int gold, std::vector<Monster> monsters);
    Dungeon() = default;
    
    std::string getName() const;
    int getGold() const;
    std::vector<Monster>& getMonsters();
    bool getCompleted();

    static std::vector<Dungeon> getDungeons(sqlite3* db);
    static Dungeon loadDungeon(sqlite3* db, const std::string& dungeonName, const std::string& heroName);

    void saveToDatabase(sqlite3* db, const std::string& heroName);
    static void deleteDungeonsForHero(sqlite3* db, const std::string& heroName);

    bool checkIfCompleted(); 
    void markAsCompleted();
    void markMonsterDefeated(const std::string& monsterName); 
};
