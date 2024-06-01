//
//  database.h
//  TerminalGame
//
//  Created by Felix Mogensen on 19/05/2024.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>

class Database {
public:
    Database(const char* dbName);
    ~Database();
    void createTable();
    void insertPlayer(int id, const std::string& name, int score);
    void selectAllPlayers();
private:
    sqlite3* db;
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
};

bool heroNameExists(sqlite3* db, const std::string& heroName);

#endif 
