#include "Hero.h"
#include <chrono>
#include <thread>

// Farver
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

Hero::Hero(const std::string& n, int x, int l, int h, int s) : name(n), xp(x), level(l), hp(h), strength(s), gold(0) {
    if (name.empty()) {
        name = "Default Hero";
    }
}

std::string Hero::getName() const {
    return name;
}

int Hero::getXP() const {
    return xp;
}

int Hero::getLevel() const {
    return level;
}

int Hero::getHP() const {
    return hp;
}

int Hero::getStrength() const {
    return strength;
}

int Hero::getGold() const {
    return gold;
}

void Hero::addGold(int amount) {
    gold += amount;
}

void Hero::displayInfo() const {
    std::cout << "Name: " << name << "\n"
              << "XP: " << xp << "\n"
              << "Level: " << level << "\n"
              << "HP: " << hp << "\n"
              << "Strength: " << strength << "\n"
              << "Gold: " << gold << "\n"
              << std::endl;
}

void Hero::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
    std::cout << "\n" << RED << name << " took " << damage << " damage. HP is now " << hp << "." << std::endl << RESET;
}

void Hero::gainXP(int experience) {
    xp += experience;
    std::cout << name << " gained " << experience << " XP. Total XP is now " << xp << "." << std::endl;
    while (xp >= (level * 1000)) {
        levelUp();
    }
}

void Hero::levelUp() {
    level++;
    hp += 7;
    strength += 2;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << GREEN << name << " leveled up! Level is now " << level << "." << "\n" << RESET << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    this->displayInfo();
}

void Hero::saveToDatabase(sqlite3* db) {
    if (defeated) {
        return; 
    }

    char* zErrMsg = nullptr;
    std::string sql = "INSERT OR REPLACE INTO Hero (Name, XP, Level, HP, Strength, Gold) VALUES ('" +
                      name + "', " + std::to_string(xp) + ", " + std::to_string(level) + ", " +
                      std::to_string(hp) + ", " + std::to_string(strength) + ", " + std::to_string(gold) + ");";
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << GREEN << "Hero saved to database successfully." << "\n" << std::endl << RESET;
    }
}

Hero Hero::loadFromDatabase(sqlite3* db, const std::string& heroName) {
    std::string sql = "SELECT * FROM Hero WHERE Name = '" + heroName + "';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return Hero(heroName);
    }

    Hero hero(heroName);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        hero.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        hero.xp = sqlite3_column_int(stmt, 1);
        hero.level = sqlite3_column_int(stmt, 2);
        hero.hp = sqlite3_column_int(stmt, 3);
        hero.strength = sqlite3_column_int(stmt, 4);
        hero.gold = sqlite3_column_int(stmt, 5);  // Load gold
        std::cout << GREEN << "Hero loaded from database successfully." << "\n" << std::endl << RESET;
    } else {
        std::cout << RED << "Hero not found in the database." << "\n" << std::endl << RESET;
    }

    sqlite3_finalize(stmt);
    return hero;
}

void Hero::deleteFromDatabase(sqlite3* db) {
    char* zErrMsg = nullptr;
    std::string sql = "DELETE FROM Hero WHERE Name = '" + name + "';";
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Hero deleted from database successfully.\n" << std::endl;
    }
    defeated = true; 
}

