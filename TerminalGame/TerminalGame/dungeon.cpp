#include "dungeon.h"
#include <iostream>
#include <vector>
#include <string>

Dungeon::Dungeon(std::string name, int gold, std::vector<Monster> monsters)
    : name(name), gold(gold), monsters(monsters) {}

std::string Dungeon::getName() const {
    return name;
}

int Dungeon::getGold() const {
    return gold;
}

std::vector<Monster>& Dungeon::getMonsters() {
    return monsters;
}

void Dungeon::saveToDatabase(sqlite3* db, const std::string& heroName) {
    std::string sql = "INSERT OR REPLACE INTO Dungeon (Name, Gold) VALUES ('" +
                      name + "', " + std::to_string(gold) + ");";

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (saving Dungeon): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Dungeon " << name << " saved to database with Gold " << gold << "." << std::endl;
    }

    for (Monster& monster : monsters) {
        monster.saveToDatabase(db, heroName, name);
    }
}

Dungeon Dungeon::loadDungeon(sqlite3* db, const std::string& dungeonName, const std::string& heroName) {
    std::string sql = "SELECT Gold FROM Dungeon WHERE Name = '" + dungeonName + "';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    int gold = 0;

    if (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        gold = sqlite3_column_int(stmt, 0);
    } else {
        std::cerr << "SQL error (loading Dungeon): " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    std::cout << "Loading monsters for hero: " << heroName << " in dungeon: " << dungeonName << std::endl;
    std::vector<Monster> monsters = Monster::loadAllFromDatabase(db, heroName, dungeonName);

    return Dungeon(dungeonName, gold, monsters);
}

std::vector<Dungeon> Dungeon::getDungeons(sqlite3* db) {
    // Define dungeons with associated monsters
    std::vector<Dungeon> dungeons = {
        Dungeon("Dungeon 1", 100, {Monster("Goblin", 4, 1, 350), Monster("Orc", 8, 3, 450)}),
        Dungeon("Dungeon 2", 200, {Monster("Monkey", 15, 4, 650), Monster("Spider", 25, 4, 900)}),
        Dungeon("Dungeon 3", 500, {Monster("Voodoo Man", 50, 8, 2000), Monster("Dragon", 100, 10, 3000)})
    };

    return dungeons;
}

void Dungeon::deleteDungeonsForHero(sqlite3* db, const std::string& heroName) {
    std::string sql = "DELETE FROM Dungeon WHERE Name IN (SELECT DISTINCT DungeonName FROM Monster WHERE HeroName = '" + heroName + "');";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error (deleting Dungeons): " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Dungeons associated with hero " << heroName << " deleted from the database." << std::endl;
    }
}

bool Dungeon::checkIfCompleted() {
    std::unordered_set<std::string> uniqueMonsters;
    for (const Monster& monster : monsters) {
        uniqueMonsters.insert(monster.getName());
    }
    return defeatedMonsters.size() == uniqueMonsters.size();
}


void Dungeon::markMonsterDefeated(const std::string& monsterName) {
    defeatedMonsters.insert(monsterName);
}

void Dungeon::markAsCompleted() {
    completed = true;
}

bool Dungeon::getCompleted(){
    return completed;
}
