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
#include <vector>
#include "database.h"
#include "Monsters.h"
#include <chrono>
#include <thread>

// Farver
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

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

bool gameLoop(Hero& hero, sqlite3* db) {
    std::vector<Monster> monsters = Monster::getMonsters();

    while (true) {
        std::cout << "Choose an action:\n";
        for (size_t i = 0; i < monsters.size(); ++i) {
            std::cout << i + 1 << ". Attack " << monsters[i].getName() << "\n";
        }
        std::cout << monsters.size() + 1 << ". Save\n";
        std::cout << "Enter your choice: ";

        int action;
        std::cin >> action;

          if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "Invalid choice. Please enter a number." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        if (action >= 1 && action <= monsters.size()) {
            Monster& monster = monsters[action - 1];
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "You attacked the " << monster.getName() << "!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            hero.takeDamage(monster.getStrength());
            monster.takeDamage(hero.getStrength());
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            monster.displayInfo();
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            if (monster.getHP() == 0) {
                std::cout << GREEN << "You defeated the " << monster.getName() << "!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                hero.gainXP(monster.getXP());

                if (monster.getName() == "Dragon") { // If the dragon is defeated, end the game
                    std::cout << GREEN << "Congratulations! You have defeated the Dragon and finished the game!\n" << RESET;
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    return true; // End the game
                }
            }
        } else if (action == monsters.size() + 1) {
            return false;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }

        if (hero.getHP() == 0) {
            std::cout << RED << "You have been defeated!\n" << RESET;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            hero.deleteFromDatabase(db);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return true;
        }
    }
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("game.db", &db);
    if (rc) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "Opened database successfully." << std::endl;
    }

    createDatabaseAndTable(db);

        while(true){
        std::cout << "Welcome to this awesome game." << std::endl;
        std::cout << "1. Create New Hero" << std::endl;
        std::cout << "2. Load Existing Hero" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose an option: ";
        
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) { // error handling
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << RED << "Invalid option. Please enter a number." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        if (choice < 1 || choice > 3) { // error handling
            std::cout << RED << "Invalid option. Please try again." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        Hero hero(""); 

        if (choice == 1) {
            std::string name;

            
            std::cout << "Enter hero name: ";
            std::cin >> name;

             if (heroNameExists(db, name)) {
                std::cout << RED << "A hero with this name already exists. Please choose a different name.\n" << std::endl << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                continue;
            }
          
            hero = Hero(name); 
            Hero newHero(name);
            newHero.saveToDatabase(db);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            newHero.displayInfo();
        } else if (choice == 2) {
            std::string name;
            std::cout << "Enter hero name to load: ";
            std::cin >> name;

            Hero loadedHero = Hero::loadFromDatabase(db, name);
             
            if (!heroNameExists(db, name)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                continue; 
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            loadedHero.displayInfo();
            hero = loadedHero; 
        } else if (choice == 3) {
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        
        bool endGame = gameLoop(hero, db);

        if (!endGame) {
            hero.saveToDatabase(db); // save the game
        } else {
            break; // end the game
        }

        }

    sqlite3_close(db);
    return 0;
}





