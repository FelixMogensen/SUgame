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

bool gameLoop(Hero& hero, std::vector<Monster>& monsters, sqlite3* db1, Dungeon chosenDungeon) {

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
                chosenDungeon.saveToDatabase(db1, hero.getName()); 
                }

                if (monster.getName() == "Dragon") { // if the dragon is defeated, end the game
                    std::cout << GREEN << "Congratulations! You have defeated the Dragon and finished the game!\n" << RESET;
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    return true; // end the game
                }
                monster.respawn(); // reset hp when defeated
                monster.saveToDatabase(db1, hero.getName(), chosenDungeon.getName()); // save 
                chosenDungeon.saveToDatabase(db1, hero.getName());
            }
        } else if (action == monsters.size() + 1) {
            return false; // save
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }

        if (hero.getHP() == 0) {
            std::cout << RED << "You have been defeated!\n" << RESET;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            hero.deleteFromDatabase(db1); // erase hero
            Monster::deleteMonstersForHero(db1, hero.getName()); // erase monsters
            Dungeon::deleteDungeonsForHero(db1, hero.getName());
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            return true;
        }
    }
}

int main() {
    Database db1("game.db");
    db1.createDatabaseAndTable(db1.getDbPointer());

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

             if (db1.heroNameExists(db1.getDbPointer(), name)) {
                std::cout << RED << "A hero with this name already exists. Please choose a different name.\n" << std::endl << RESET;
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                continue;
            }

            std::vector<Dungeon> dungeons = Dungeon::getDungeons(db1.getDbPointer());
            std::cout << "Choose a dungeon:\n";
            for (size_t i = 0; i < dungeons.size(); ++i) {
                std::cout << i + 1 << ". " << dungeons[i].getName() << " (Gold: " << dungeons[i].getGold() << ")\n";
            }
            int dungeonChoice;
            std::cin >> dungeonChoice;

            chosenDungeon = dungeons[dungeonChoice - 1];
            monsters = chosenDungeon.getMonsters();
          
            hero = Hero(name);
            hero.saveToDatabase(db1.getDbPointer());

            for (Dungeon& dungeon : dungeons) {
            dungeon.saveToDatabase(db1.getDbPointer(), hero.getName()); // save monsters 
            }

            hero.displayInfo();

        } else if (choice == 2) { // load existing game
            std::string name;
            std::cout << "Enter hero name to load: ";
            std::cin >> name;

            Hero loadedHero = Hero::loadFromDatabase(db1.getDbPointer(), name);
            hero = loadedHero; 

             if (!db1.heroNameExists(db1.getDbPointer(), name)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue; 
            }

            std::vector<Dungeon> dungeons = Dungeon::getDungeons(db1.getDbPointer());
            std::cout << "Choose a dungeon:\n";
            for (size_t i = 0; i < dungeons.size(); ++i) {
                std::cout << i + 1 << ". " << dungeons[i].getName() << " (Gold: " << dungeons[i].getGold() << ")\n";
            }
            int dungeonChoice;
            std::cin >> dungeonChoice;

            std::cout << "Loading dungeon with hero: " << hero.getName() << std::endl;

            chosenDungeon = Dungeon::loadDungeon(db1.getDbPointer(), dungeons[dungeonChoice - 1].getName(), hero.getName());
            monsters = chosenDungeon.getMonsters();

            loadedHero.displayInfo();

        } else if (choice == 3) { // shut down
            return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        bool endGame = gameLoop(hero, monsters, db1.getDbPointer(), chosenDungeon);

        if (!endGame) {
            hero.saveToDatabase(db1.getDbPointer()); // save the hero
                 for (Monster& monster : monsters) {
                 monster.saveToDatabase(db1.getDbPointer(), hero.getName(), chosenDungeon.getName()); // save monsters
                 }
        } else {
            break; // end the game
        }

        }

    sqlite3_close(db1.getDbPointer());
    return 0;
}