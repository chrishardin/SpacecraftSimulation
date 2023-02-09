#include "Vector3.h"

#include "random_gen.h"

#include <map>
#include <vector>

const double epsilon = 0.001;

class Database;
class Spacecraft;
class System;

struct PlanetInitializationData
{
    PlanetInitializationData() {}
    ~PlanetInitializationData() {}

    std::string systemName;
    std::string name;
    double radius;
    double mass;
    double posX;
    double posY;
    double posZ;
    double gravParam;
    double atmoRadius;
};

struct SystemInitializationData
{
    SystemInitializationData() {}
    ~SystemInitializationData() {}

    std::string name;
};

struct SpacecraftInitializationData
{
    SpacecraftInitializationData() {}
    ~SpacecraftInitializationData() {}

    std::string name;
    double area;
    double mass;
    double angularVelocity;
    double maxVelocity;
    double targetVelX;
    double targetVelY;
    double targetVelZ;
    double targetAccX;
    double targetAccY;
    double targetAccZ;
};


typedef Vector3<double> VecDouble;


class Scenario
{
    public:
        Scenario();
        ~Scenario();
        bool loadFiles();
        bool compile();
        int runSimulation();

        void addSpacecraft(Spacecraft* spacecraft);
        void addSystem(System* system);

        Database* getDatabase() { return _database; }

        std::map<std::string, Spacecraft*>& getSpacecraft() { return _spacecraft; }
        std::map<std::string, System*>& getSystems() { return _systems; }

        bool equalVectors(Vector3<double> a, Vector3<double> b) {
            return fabs(a.x - b.x) < epsilon
                && fabs(a.y - b.y) < epsilon
                && fabs(a.z - b.z) < epsilon;
        }

    protected:

        bool loadSystemsFromFile(const std::string& filepath);
        bool loadPlanetsFromFile(const std::string& filepath);
        bool loadSpacecraftFromFile(const std::string& filepath);

        std::vector<SpacecraftInitializationData>& getSpacecraftInitData() { return _spacecraftInitData; }
        std::vector<SystemInitializationData>& getSystemInitData() { return _systemInitData; }
        std::vector<PlanetInitializationData>& getPlanetInitData() { return _planetInitData; }

        double _currentTimestamp;
        
        Database* _database;

        std::map<std::string, Spacecraft*> _spacecraft;
        std::map<std::string, System*> _systems;
        std::map<std::string, std::string> _filepaths;

        std::vector<SpacecraftInitializationData> _spacecraftInitData;
        std::vector<SystemInitializationData> _systemInitData;
        std::vector<PlanetInitializationData> _planetInitData;
};

