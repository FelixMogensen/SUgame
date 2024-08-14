#pragma once
#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* db;
    static int callback(void* NotUsed, int argc, char** argv, char** azColName);
public:
    Database(const char* dbName);
    ~Database();
    void createTable();
    void insertPlayer(int id, const std::string& name, int score);

    bool heroNameExists(sqlite3* db, const std::string& heroName);
};