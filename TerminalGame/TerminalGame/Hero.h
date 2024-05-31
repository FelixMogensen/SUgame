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

public:
    Hero(std::string n, int x = 0, int l = 1, int h = 100, int s = 10)
        : name(n), xp(x), level(l), hp(h), strength(s) {}

    std::string getName() const { return name; }
    int getXP() const { return xp; }
    int getLevel() const { return level; }
    int getHP() const { return hp; }
    int getStrength() const { return strength; }

    void setName(std::string n) { name = n; }
    void setXP(int x) { xp = x; }
    void setLevel(int l) { level = l; }
    void setHP(int h) { hp = h; }
    void setStrength(int s) { strength = s; }

    void displayInfo() const {
        std::cout << "Name: " << name << "\n"
                  << "XP: " << xp << "\n"
                  << "Level: " << level << "\n"
                  << "HP: " << hp << "\n"
                  << "Strength: " << strength << std::endl;
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;
        std::cout << name << " took " << damage << " damage. HP is now " << hp << "." << std::endl;
    }

    void gainXP(int experience) {
        xp += experience;
        std::cout << name << " gained " << experience << " XP. Total XP is now " << xp << "." << std::endl;
        if (xp >= level * 100) {
            levelUp();
        }
    }

    void saveToDatabase(sqlite3* db) {
        char* zErrMsg = nullptr;
        std::string sql = "INSERT INTO Hero (Name, XP, Level, HP, Strength) VALUES ('" +
                          name + "', " + std::to_string(xp) + ", " + std::to_string(level) + ", " +
                          std::to_string(hp) + ", " + std::to_string(strength) + ");";
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        } else {
            std::cout << "Hero saved to database successfully." << std::endl;
        }
    }

    static Hero loadFromDatabase(sqlite3* db, const std::string& heroName) {
        std::string sql = "SELECT * FROM Hero WHERE Name = '" + heroName + "';";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
            return Hero("");
        }

        Hero hero("");
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            hero.name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
            hero.xp = sqlite3_column_int(stmt, 1);
            hero.level = sqlite3_column_int(stmt, 2);
            hero.hp = sqlite3_column_int(stmt, 3);
            hero.strength = sqlite3_column_int(stmt, 4);
            std::cout << "Hero loaded from database successfully." << std::endl;
        } else {
            std::cout << "Hero not found in the database." << std::endl;
        }

        sqlite3_finalize(stmt);
        return hero;
    }

private:
    void levelUp() {
        level++;
        hp += 20;
        strength += 5;
        std::cout << name << " leveled up! Level is now " << level << "." << std::endl;
    }
};
