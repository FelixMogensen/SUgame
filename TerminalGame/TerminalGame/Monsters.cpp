#include <iostream>
#include <string>
#include "Monsters.h"
#include <vector>

Monster::Monster(std::string name, int hp, int strength, int xp) : name(name), hp(hp), strength(strength), xp(xp), originalhp(hp) {}

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

void Monster::respawn(){
    hp = originalhp;
}

std::vector<Monster> Monster::getMonsters(sqlite3* db) { // standard monsters for initiation
    std::vector<Monster> monsters = {
        Monster("Goblin", 4, 1, 350),
        Monster("Orc", 8, 3, 450),
        Monster("Monkey", 15, 4, 650),
        Monster("Spider", 25, 4, 900),
        Monster("Dragon", 100, 10, 3000)
    };
    return monsters;
}

void Monster::displayInfo() const {
    std::cout << "Monster " << name << " with remaning HP: " << hp << "\n";
}

void Monster::saveToDatabase(sqlite3* db, const std::string& heroName) {
    std::string sql = "INSERT OR REPLACE INTO Monster (Name, HP, HeroName) VALUES ('" +
                      name + "', " + std::to_string(hp) + ", '" + heroName + "');";

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Monster " << name << " saved to database with HeroName " << heroName << "." << std::endl;
    }
}

std::vector<Monster> Monster::loadAllFromDatabase(sqlite3* db, const std::string& heroName) {
    std::vector<Monster> monsters;
    std::string sql = "SELECT Name, HP FROM Monster WHERE HeroName = '" + heroName + "';";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return monsters; // return an empty vector on error
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        int hp = sqlite3_column_int(stmt, 1);

        // Set default attributes for the monsters as before
        int strength = 0;
        int xp = 0;
        int originalhp = 0;

        if (name == "Goblin") {
            strength = 1;
            xp = 350;
            originalhp = 4;
        } else if (name == "Orc") {
            strength = 3;
            xp = 450;
            originalhp = 8;
        } else if (name == "Monkey") {
            strength = 4;
            xp = 650;
            originalhp = 15;
        } else if (name == "Spider") {
            strength = 4;
            xp = 900;
            originalhp = 25;
        } else if (name == "Dragon") {
            strength = 10;
            xp = 3000;
            originalhp = 100;
        }

        Monster monster(name, originalhp, strength, xp);
        monster.hp = hp;  // set current HP

        monsters.push_back(monster); // add the loaded monster to the vector
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error during step: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return monsters; // return the loaded monsters
}


void Monster::deleteMonstersForHero(sqlite3* db, const std::string& heroName) {
    std::string sql = "DELETE FROM Monster WHERE HeroName = '" + heroName + "';";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Monsters for hero " << heroName << " deleted from database." << std::endl;
    }
}