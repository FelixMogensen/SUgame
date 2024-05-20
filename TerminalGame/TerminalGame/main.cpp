//
//  main.cpp
//  TerminalGame
//
//  Created by Felix Mogensen on 19/05/2024.
//

#include <iostream>
#include "database.h"

int main() {
    std::cout << "Welcome to the Terminal Game!" << std::endl;

    Database db("game.db");
    db.createTable();
    db.insertPlayer(1, "Player1", 100);
    db.selectAllPlayers();
    
    

    // Add your game logic here

    return 0;
}
