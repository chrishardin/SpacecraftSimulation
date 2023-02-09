#include "utility/sqlite3.h"

#include "Vector3.h"

#include <string>
#include <vector>

class Planet;

class Database
{
    public:
        Database(const std::string& filename);
        ~Database();
        
        void runSQL(std::string& sql, std::string& errorString);
        //int runSQL(const char* sql, std::string& errorString);

        void dropTables();
        void createTables();
        void logSimData(double time, Vector3<double> position, Vector3<double> velocity, Vector3<double> acceleration);
        void logPlanetData(Planet* planet);

        template <typename T>
        int bindValue(sqlite3_stmt* stmt, int index, const T& value) 
        {
            if constexpr (std::is_same_v<T, int>) 
            {
                return sqlite3_bind_int(stmt, index, value);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return sqlite3_bind_double(stmt, index, value);
            }
            else if constexpr (std::is_same_v<T, std::string>) 
            {
                return sqlite3_bind_text(stmt, index, value.c_str(), value.size(),
                    SQLITE_TRANSIENT);
            }
            else 
            {
                std::cerr << "Error trying to bindValue index = "+ std::to_string(index) + " value = " + value << std::endl;
            }
        }

    protected:
        std::string _filename;
        sqlite3* _database;
};

