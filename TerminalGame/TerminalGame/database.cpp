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

void Database::createDatabaseAndTable(sqlite3* db) {
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

bool Database::heroNameExists(sqlite3* db, const std::string& heroName) {
    std::string sql = "SELECT COUNT(*) FROM Hero WHERE Name = '" + heroName + "';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return true; 
    }

    rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0);

    sqlite3_finalize(stmt);
    return exists;
}