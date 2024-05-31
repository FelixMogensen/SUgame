cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TerminalGame VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Manually set the SQLite3 include and library directories
set(SQLITE3_INCLUDE_DIR /opt/homebrew/opt/sqlite/include)
set(SQLITE3_LIBRARY /opt/homebrew/opt/sqlite/lib/libsqlite3.dylib)

# Include directories for SQLite3
include_directories(${SQLITE3_INCLUDE_DIR})

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Add an executable
add_executable(TerminalGame TerminalGame/TerminalGame/main.cpp TerminalGame/TerminalGame/database.cpp TerminalGame/TerminalGame/Monsters.cpp TerminalGame/TerminalGame/Hero.cpp)

# Link SQLite library
target_link_libraries(TerminalGame SQLite::SQLite3)