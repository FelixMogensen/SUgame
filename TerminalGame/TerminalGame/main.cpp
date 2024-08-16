#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Hero.h"
#include <vector>
#include "database.h"
#include "Monsters.h"
#include "dungeon.h"
#include <chrono>
#include <thread>

// Farver
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";

bool gameLoop(Hero& hero, std::vector<Monster>& monsters, sqlite3* db, Dungeon chosenDungeon) {

    while (true) {
        std::cout << "Choose an action:\n";
        for (size_t i = 0; i < monsters.size(); ++i) {
            std::cout << i + 1 << ". Attack " << monsters[i].getName() << "   (hp: " << monsters[i].getHP() << " -- strength: " << monsters[i].getStrength() << ")\n";
        }
        std::cout << monsters.size() + 1 << ". Save\n";
        std::cout << "Enter your choice: ";

        int action;
        std::cin >> action;

          if (std::cin.fail()) { // error handling
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "Invalid choice. Please enter a number." << RESET << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            continue;
        }

        if (action >= 1 && action <= monsters.size()) {
            Monster& monster = monsters[action - 1];
            std::cout << "You attacked the " << monster.getName() << "!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            hero.takeDamage(monster.getStrength());
            monster.takeDamage(hero.getStrength());
            monster.displayInfo();
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            if (monster.getHP() == 0) {
                std::cout << GREEN << "You defeated the " << monster.getName() << "!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                hero.gainXP(monster.getXP());

                chosenDungeon.markMonsterDefeated(monster.getName());

                  if (chosenDungeon.checkIfCompleted() and !chosenDungeon.getCompleted()) {
                std::cout << GREEN << "Congratulations! You have completed " << chosenDungeon.getName() << "!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                hero.addGold(chosenDungeon.getGold());
                std::cout << GREEN << "You earned " << chosenDungeon.getGold() << " gold!\n" << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                chosenDungeon.markAsCompleted();
                }

                if (monster.getName() == "Dragon") { // if the dragon is defeated, end the game
                    std::cout << GREEN << "Congratulations! You have defeated the Dragon and finished the game!\n" << RESET;
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    return true; // end the game
                }
                monster.respawn(); // reset hp when defeated
                monster.saveToDatabase(db, hero.getName(), chosenDungeon.getName()); // save 
            }
        } else if (action == monsters.size() + 1) {
            return false; // save
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }

        if (hero.getHP() == 0) {
            std::cout << RED << "You have been defeated!\n" << RESET;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            hero.deleteFromDatabase(db); // erase hero
            Monster::deleteMonstersForHero(db, hero.getName()); // erase monsters
            Dungeon::deleteDungeonsForHero(db, hero.getName());
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

    Database db1("game.db");
    db1.createDatabaseAndTable(db);

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

        Hero hero(""); // instantiation
        std::vector<Monster> monsters; //instantiation
        Dungeon chosenDungeon;

        if (choice == 1) { // new game
            std::string name;

            std::cout << "Enter hero name: ";
            std::cin >> name;

             if (db1.heroNameExists(db, name)) {
                std::cout << RED << "A hero with this name already exists. Please choose a different name.\n" << std::endl << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                continue;
            }

            std::vector<Dungeon> dungeons = Dungeon::getDungeons(db);
            std::cout << "Choose a dungeon:\n";
            for (size_t i = 0; i < dungeons.size(); ++i) {
                std::cout << i + 1 << ". " << dungeons[i].getName() << " (Gold: " << dungeons[i].getGold() << ")\n";
            }
            int dungeonChoice;
            std::cin >> dungeonChoice;

            chosenDungeon = dungeons[dungeonChoice - 1];
            monsters = chosenDungeon.getMonsters();
          
            hero = Hero(name);
            hero.saveToDatabase(db);

            for (Dungeon& dungeon : dungeons) {
            dungeon.saveToDatabase(db, hero.getName()); // save monsters 
            }

            hero.displayInfo();

        } else if (choice == 2) { // load existing game
            std::string name;
            std::cout << "Enter hero name to load: ";
            std::cin >> name;

            Hero loadedHero = Hero::loadFromDatabase(db, name);
            hero = loadedHero; 

             if (!db1.heroNameExists(db, name)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue; 
            }

            std::vector<Dungeon> dungeons = Dungeon::getDungeons(db);
            std::cout << "Choose a dungeon:\n";
            for (size_t i = 0; i < dungeons.size(); ++i) {
                std::cout << i + 1 << ". " << dungeons[i].getName() << " (Gold: " << dungeons[i].getGold() << ")\n";
            }
            int dungeonChoice;
            std::cin >> dungeonChoice;

            std::cout << "Loading dungeon with hero: " << hero.getName() << std::endl;

            chosenDungeon = Dungeon::loadDungeon(db, dungeons[dungeonChoice - 1].getName(), hero.getName());
            monsters = chosenDungeon.getMonsters();

            loadedHero.displayInfo();

        } else if (choice == 3) { // shut down
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        bool endGame = gameLoop(hero, monsters, db, chosenDungeon);

        if (!endGame) {
            hero.saveToDatabase(db); // save the hero
                 for (Monster& monster : monsters) {
                 monster.saveToDatabase(db, hero.getName(), chosenDungeon.getName()); // save monsters
                 }
        } else {
            break; // end the game
        }

        }

    sqlite3_close(db);
    return 0;
}