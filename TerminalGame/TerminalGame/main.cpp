//
//  main.cpp
//  TerminalGame
//
//  Created by Felix Mogensen on 19/05/2024.
//

#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Hero.h"
#include "database.h"


void createDatabaseAndTable(sqlite3* db) {
    char* zErrMsg = nullptr;
    const char* sql = "CREATE TABLE IF NOT EXISTS Hero ("
                      "Name TEXT PRIMARY KEY NOT NULL,"
                      "XP INT NOT NULL,"
                      "Level INT NOT NULL,"
                      "HP INT NOT NULL,"
                      "Strength INT NOT NULL);";
    int rc = sqlite3_exec(db, sql, nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table created successfully." << std::endl;
    }
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("game.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Opened database successfully." << std::endl;
    }

    createDatabaseAndTable(db);

        std::cout << "Welcome to this awesome game." << std::endl;
        std::cout << "1. Create New Hero" << std::endl;
        std::cout << "2. Load Existing Hero" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose an option: ";
        
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::string name;
            std::cout << "Enter hero name: ";
            std::cin >> name;

            Hero newHero(name);
            newHero.saveToDatabase(db);
            newHero.displayInfo();
        } else if (choice == 2) {
            std::string name;
            std::cout << "Enter hero name to load: ";
            std::cin >> name;

            Hero loadedHero = Hero::loadFromDatabase(db, name);
            loadedHero.displayInfo();
        } else if (choice == 3) {
            return 0;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    

    sqlite3_close(db);
    return 0;
}



