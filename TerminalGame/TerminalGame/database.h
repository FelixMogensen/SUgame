#pragma once
#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* db;
public:
    Database(const char* dbName);
    void createDatabaseAndTable(sqlite3* db);
    bool heroNameExists(sqlite3* db, const std::string& heroName);
};