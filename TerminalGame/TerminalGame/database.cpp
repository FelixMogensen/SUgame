//
//  database.cpp
//  TerminalGame
//
//  Created by Felix Mogensen on 19/05/2024.
//

#include "database.h"
#include <iostream>

Database::Database(const char* dbName) {
    int rc = sqlite3_open(dbName, &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        std::cout << "Opened database successfully" << std::endl;
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

void Database::createTable() {
    if (!db) return;

    const char* sql = "CREATE TABLE PLAYER("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "NAME TEXT NOT NULL,"
                      "SCORE INT NOT NULL);";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }
}

void Database::insertPlayer(int id, const std::string& name, int score) {
    if (!db) return;

    std::string sql = "INSERT INTO PLAYER (ID, NAME, SCORE) VALUES (" +
                      std::to_string(id) + ", '" + name + "', " +
                      std::to_string(score) + ");";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << "Records created successfully" << std::endl;
    }
}

void Database::selectAllPlayers() {
    if (!db) return;

    const char* sql = "SELECT * FROM PLAYER;";
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

int Database::callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
