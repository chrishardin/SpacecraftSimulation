#include "Database.h"

#include "Planet.h"
#include "Vector3.h"

#include <iostream>

const char* input_planet = "INSERT INTO InputPlanets(systemName, planetName, centerPosition_x, centerPosition_y, centerPosition_z, radius, mass, initialGravitationalParameter, initialAirTemperature, initialDragCoefficient) VALUES(?,?,?,?,?,?,?,?,?,?)";
const char* input_systems = "INSERT INTO InputSystems(systemName) VALUES(?)";
const char* input_spacecraft = "INSERT INTO InputSpacecraft(name, area, mass, angularVelocity, maxVelocity, targetVelocityX, targetVelocityY, targetVelocitZ, targetAccelerationX, targetAccelerationY, targetAccelerationZ) VALUES(?,?,?,?,?,?,?,?,?,?,?)";

const char* simulation_data = "INSERT INTO simulation_data(time, position_x, position_y, position_z, velocity_x, velocity_y, velocity_z, acceleration_x, acceleration_y, acceleration_z) VALUES(?,?,?,?,?,?,?,?,?,?)";

Database::Database(const std::string & filename) : _filename(filename)
{
    int result = sqlite3_open(_filename.c_str(), &_database);

    if (result)
    {
        std::cerr << "Can't open database: " << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

    dropTables();
}

Database::~Database()
{
    int result = sqlite3_close(_database);
    
    if (result != SQLITE_OK)
    {
        std::cerr << "Can't close database: " << sqlite3_errmsg(_database) << std::endl;
    }

    std::cout << "Created database file " << _filename << std::endl;
}

void Database::runSQL(std::string& sql, std::string& errorString)
{
    char* errMsg = 0;
    int result = sqlite3_exec(_database, sql.c_str(), nullptr, nullptr, &errMsg);
    if (result != SQLITE_OK)
    {
        std::cerr << errorString << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}


void Database::dropTables()
{
    std::string input_planets = "DROP TABLE IF EXISTS InputPlanets;";
    std::string input_systems = "DROP TABLE IF EXISTS InputSystems;";
    std::string input_spacecraft = "DROP TABLE IF EXISTS InputSpacecraft;";
    std::string simulation_data = "DROP TABLE IF EXISTS simulation_data;";
    std::string errorString = "Error dropping table: ";

    runSQL(input_planets, errorString);
    runSQL(input_systems, errorString);
    runSQL(input_spacecraft, errorString);
    runSQL(simulation_data, errorString);
}

void Database::createTables()
{
    std::string input_planets =
        "CREATE TABLE IF NOT EXISTS InputPlanets("
        "systemName TEXT NOT NULL,"
        "planetName TEXT NOT NULL,"
        "centerPosition_x REAL NOT NULL,"
        "centerPosition_y REAL NOT NULL,"
        "centerPosition_z REAL NOT NULL,"
        "radius REAL NOT NULL,"
        "mass REAL NOT NULL,"
        "initialGravitationalParameter REAL NOT NULL,"
        "initialAirTemperature REAL NOT NULL,"
        "initialDragCoefficient REAL NOT NULL,"
        "PRIMARY KEY (systemName, planetName)"
        ");";

    std::string input_systems =
        "CREATE TABLE IF NOT EXISTS InputSystems("
        "systemName TEXT NOT NULL,"
        "PRIMARY KEY (systemName)"
        ");";

    std::string input_spacecraft =
        "CREATE TABLE IF NOT EXISTS InputSpacecraft("
        "name TEXT NOT NULL,"
        "area REAL NOT NULL,"
        "mass REAL NOT NULL,"
        "angularVelocity REAL NOT NULL,"
        "maxVelocity REAL NOT NULL,"
        "targetVelocityX REAL NOT NULL,"
        "targetVelocityY REAL NOT NULL,"
        "targetVelocityZ REAL NOT NULL,"
        "targetAccelerationX REAL NOT NULL,"
        "targetAccelerationY REAL NOT NULL,"
        "targetAccelerationZ REAL NOT NULL,"
        "PRIMARY KEY (name)"
        ");";

    std::string simulation_data =
        "CREATE TABLE IF NOT EXISTS simulation_data("
        "time INT NOT NULL,"
        "position_x REAL NOT NULL,"
        "position_y REAL NOT NULL,"
        "position_z REAL NOT NULL,"
        "velocity_x REAL NOT NULL,"
        "velocity_y REAL NOT NULL,"
        "velocity_z REAL NOT NULL,"
        "acceleration_x REAL NOT NULL,"
        "acceleration_y REAL NOT NULL,"
        "acceleration_z REAL NOT NULL,"
        "PRIMARY KEY (time)"
        ");";

    std::string errorString = "Error creating table: ";

    runSQL(input_planets, errorString);
    runSQL(input_systems, errorString);
    runSQL(input_spacecraft, errorString);
    runSQL(simulation_data, errorString);
}

void Database::logSimData(double time, Vector3<double> position, Vector3<double> velocity, Vector3<double> acceleration)
{
    std::string errorString = "Error inserting into table: ";

    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(_database, simulation_data, -1, &stmt, NULL);

    if (result != SQLITE_OK)
    {
        std::cerr << errorString << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

    bindValue(stmt, 1, time);
    bindValue(stmt, 2, position.x);
    bindValue(stmt, 3, position.y);
    bindValue(stmt, 4, position.z);
    bindValue(stmt, 5, velocity.x);
    bindValue(stmt, 6, velocity.y);
    bindValue(stmt, 7, velocity.z);
    bindValue(stmt, 8, acceleration.x);
    bindValue(stmt, 9, acceleration.y);
    bindValue(stmt, 10, acceleration.z);

    // Execute the statement
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE)
    {
        std::cerr << "Can't insert data: " << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

    // Finalize the statement
    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK)
    {
        std::cerr << "Can't finalize statement: " << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }
}


void Database::logPlanetData(Planet* planet)
{
    std::string errorString = "Error inserting into table: ";

    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(_database, input_planet, -1, &stmt, NULL);

    if (result != SQLITE_OK)
    {
        std::cerr << errorString << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

    bindValue(stmt, 1, planet->getSystemName());
    bindValue(stmt, 2, planet->getName());
    bindValue(stmt, 3, planet->getCenterPosition().x);
    bindValue(stmt, 4, planet->getCenterPosition().y);
    bindValue(stmt, 5, planet->getCenterPosition().z);
    bindValue(stmt, 6, planet->getRadius());
    bindValue(stmt, 7, planet->getMass());
    bindValue(stmt, 8, planet->getGravititationParameter());
    bindValue(stmt, 9, planet->GetAirTemperature());
    bindValue(stmt, 10, planet->getDragCoefficientParameter());

    // Execute the statement
    result = sqlite3_step(stmt);
    if (result != SQLITE_DONE)
    {
        std::cerr << "Can't insert data: " << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

    // Finalize the statement
    result = sqlite3_finalize(stmt);
    if (result != SQLITE_OK)
    {
        std::cerr << "Can't finalize statement: " << sqlite3_errmsg(_database) << std::endl;
        sqlite3_close(_database);
    }

}
