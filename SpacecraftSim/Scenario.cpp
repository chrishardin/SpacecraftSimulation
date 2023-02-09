#include "Scenario.h"

#include "Database.h"
#include "Spacecraft.h"
#include "System.h"
#include "CSVParser.h"

#include <iostream>

Scenario::Scenario()
{
    _database = new Database("spacecraft_simulation.db");

    _filepaths["SystemsPath"] = "Systems.csv";
    _filepaths["PlanetsPath"] = "Planets.csv";
    _filepaths["SpacecraftPath"] = "Spacecraft.csv";
}

Scenario::~Scenario()
{
    for (const auto& kv : _spacecraft)
    {
        delete kv.second;
    }

    for (const auto& kv : _systems)
    {
        delete kv.second;
    }

    delete _database;
}

bool Scenario::loadFiles()
{
    for (const auto& kv : _filepaths)
    {
        auto filepath = kv.second;

        if (filepath.find("Systems.csv") != std::string::npos && !loadSystemsFromFile(filepath))
        {
            throw std::runtime_error("Could not load systems from file. filepath = " + filepath);
        }

        if (filepath.find("Planets.csv") != std::string::npos && !loadPlanetsFromFile(filepath))
        {
            throw std::runtime_error("Could not load planets from file. filepath = " + filepath);
        }

        if (filepath.find("Spacecraft.csv") != std::string::npos && !loadSpacecraftFromFile(filepath))
        {
            throw std::runtime_error("Could not load spacecraft from file. filepath = " + filepath);
        }
    }

    return true;
}
    

bool Scenario::compile()
{
    // Systems
    for (const auto& systemData : _systemInitData)
    {
        auto system = new System(systemData.name);

        addSystem(system);
    }

    // Planets
    for (const auto& planetData : _planetInitData)
    {
        // system name does not exist
        if (_systems.find(planetData.systemName) == _systems.end())
        {
            std::cout << "System name = " + planetData.systemName + " does not exist. Could not add planet name = " + planetData.name + " to scenario." << std::endl;
            return false;
        }

        auto planet = new Planet(planetData.systemName, planetData.name, planetData.radius, planetData.mass, Vector3<double>(planetData.posX, planetData.posY, planetData.posZ));

        planet->setGravitationalParameter(planetData.gravParam);
        planet->setAtmosphereRadius(planetData.atmoRadius);

        _systems.at(planetData.systemName)->addPlanet(planet);
    }

    // Spacecraft
    for (const auto& spacecraftData : _spacecraftInitData)
    {
        auto spacecraft = new Spacecraft(this, spacecraftData.name);

        spacecraft->setArea(spacecraftData.area);
        spacecraft->setMass(spacecraftData.mass);
        spacecraft->setAngularVelocity(spacecraftData.angularVelocity);
        spacecraft->setMaxVelocity(spacecraftData.maxVelocity);
        //spacecraft->setTargetVelocity(Vector3<double>(spacecraftData.targetVelX, spacecraftData.targetVelY, spacecraftData.targetVelZ));
        //spacecraft->setTargetAcceleration(Vector3<double>(spacecraftData.targetAccX, spacecraftData.targetAccY, spacecraftData.targetAccZ));

        addSpacecraft(spacecraft);
    }

    return true;
}


// return code not 0 means error
// 1 : There is not just 1 spacecraft loaded into scenario.
int Scenario::runSimulation()
{
    for (const auto& kv2 : _systems)
    {
        for (const auto& kv : kv2.second->getPlanets())
        {
            auto planet = kv.second;

            _database->logPlanetData(planet);
        }
    }

    std::cout << " - Constructing Spacecraft..." << std::endl;
    Spacecraft* spacecraft;

    if (getSpacecraft().size() == 1)
    {
        spacecraft = getSpacecraft().begin()->second;
    }
    else
    {
        std::cout << "getSpacecraft().size() != 1. Ending simulation." << std::endl;
        return 1;
    }

    std::cout << " - Running Continuous Simulation Loop..." << std::endl;

    // set home planet for spacecraft
    auto planet = _systems.at("Pok'Tul Zar")->getPlanets().at("Smeg");
    spacecraft->setPlanetInformation(planet);

    // set target for spacecraft
    planet = _systems.at("Pok'Tul Zar")->getPlanets().at("Tha Nal");
    spacecraft->setTargetPlanet(planet);

    int t = 0;
    //while  (spacecraft->getPosition().magnitude() < spacecraft->getPlanet().getRadius() + 100e3)
    while(!equalVectors(spacecraft->getPosition(), spacecraft->getTargetPlanet().targetPosition) && t < 100)
    {

        // std::cout << "Time: " << t << std::endl;
        spacecraft->update(1);

#if 0
        std::cout << "Time: " << t << std::endl;
        std::cout << "Position: " << spacecraft->getPosition().x << ", " << spacecraft->getPosition().y << ", " << spacecraft->getPosition().z << std::endl;
        std::cout << "Velocity: " << spacecraft->getVelocity().x << ", " << spacecraft->getVelocity().y << ", " << spacecraft->getVelocity().z << std::endl;
        std::cout << "Acceleration: " << spacecraft->getAcceleration().x << ", " << spacecraft->getAcceleration().y << ", " << spacecraft->getAcceleration().z << std::endl;
        std::cout << "Orientation: " << spacecraft->getOrientation().x << ", " << spacecraft->getOrientation().y << ", " << spacecraft->getOrientation().z << std::endl;
        std::cout << "Thrust: " << spacecraft->getThrust().x << ", " << spacecraft->getThrust().y << ", " << spacecraft->getThrust().z << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;
#endif
        _database->logSimData(t, spacecraft->getPosition(), spacecraft->getVelocity(), spacecraft->getAcceleration());

        t += 1;
    }

    std::cout << " - Finished Continuous Simulation Loop..." << std::endl;
    return 0;
}

void Scenario::addSpacecraft(Spacecraft* spacecraft)
{
    _spacecraft.emplace(spacecraft->getName(), spacecraft);
}

void Scenario::addSystem(System* system)
{
    _systems.emplace(system->getName(), system);
}

/**
 * Loads system initialization data from a CSV file.
 *
 * @param filepath The path to the CSV file.
 *
 * @return True if the file was loaded successfully, false otherwise.
 */
bool Scenario::loadSystemsFromFile(const std::string& filepath)
{
    // Create a CSVParser object
    CSVParser parser = CSVParser(filepath);

    // Verify that the file is the expected file
    parser.verifyFile("Systems.csv");

    // Skip the first line
    parser.next();

    // Verify the headers in the file
    std::vector<std::string> headers = { "name" };
    parser.verifyHeaders(headers);

    // Skip the headers line
    parser.next();

    // Loop through the remaining lines (rows) in the file
    while (parser.isValid())
    {
        // Get the current row
        auto row = parser.getRow();

        // Create a init object
        auto systemInit = SystemInitializationData();

        // Read in data from row
        systemInit.name = row[0];

        // Add the init object
        _systemInitData.push_back(systemInit);
    }

    // Return true to indicate that the file was loaded successfully
    return true;
}


/**
 * Loads planet initialization data from a CSV file.
 *
 * @param filepath The path to the CSV file.
 *
 * @return True if the file was loaded successfully, false otherwise.
 */
bool Scenario::loadPlanetsFromFile(const std::string & filepath)
{
    // Create a CSVParser object
    CSVParser parser = CSVParser(filepath);

    // Verify that the file is the expected file
    parser.verifyFile("Planets.csv");

    // Skip the first line
    parser.next();

    // Verify the headers in the file
    std::vector<std::string> headers = { "systemName","name","radius","mass","positionX","positionY","positionZ","gravityParameter","atmosphereRadius" };
    parser.verifyHeaders(headers);

    // Skip the headers line
    parser.next();

    // Loop through the remaining lines (rows) in the file
    while (parser.isValid())
    {
        // Get the current row
        auto row = parser.getRow();

        // Create a init object
        auto planetInit = PlanetInitializationData();

        // Read in data from row
        planetInit.systemName = row[0];
        planetInit.name = row[1];
        planetInit.radius = parser.convertRowToDouble(row[2]);
        planetInit.mass = parser.convertRowToDouble(row[3]);
        planetInit.posX = parser.convertRowToDouble(row[4]);
        planetInit.posY = parser.convertRowToDouble(row[5]);
        planetInit.posZ = parser.convertRowToDouble(row[6]);
        planetInit.gravParam = parser.convertRowToDouble(row[7]);
        planetInit.atmoRadius = parser.convertRowToDouble(row[8]);

        // Add the init object
        _planetInitData.push_back(planetInit);
    }

    // Return true to indicate that the file was loaded successfully
    return true;
}


/**
 * Loads spacecraft initialization data from a CSV file.
 *
 * @param filepath The path to the CSV file.
 *
 * @return True if the file was loaded successfully, false otherwise.
 */
bool Scenario::loadSpacecraftFromFile(const std::string & filepath)
{
    // Create a CSVParser object
    CSVParser parser = CSVParser(filepath);

    // Verify that the file is the expected file
    parser.verifyFile("Spacecraft.csv");

    // Skip the first line
    parser.next();

    // Verify the headers in the file
    std::vector<std::string> headers = { "name","area","mass","angularVelocity","maxVelocity","targetVelocityX","targetVelocityY","targetVelocitZ","targetAccelerationX","targetAccelerationY","targetAccelerationZ" };
    parser.verifyHeaders(headers);

    // Skip the headers line
    parser.next();

    // Loop through the remaining lines (rows) in the file
    while (parser.isValid())
    {
        // Get the current row
        auto row = parser.getRow();

        // Create init object
        auto spacecraftInit = SpacecraftInitializationData();

        // Read in data from row
        spacecraftInit.name = row[0];
        spacecraftInit.area = parser.convertRowToDouble(row[1]);
        spacecraftInit.mass = parser.convertRowToDouble(row[2]);;
        spacecraftInit.angularVelocity = parser.convertRowToDouble(row[3]);;
        spacecraftInit.maxVelocity = parser.convertRowToDouble(row[4]);;
        spacecraftInit.targetVelX = parser.convertRowToDouble(row[5]);;
        spacecraftInit.targetVelY = parser.convertRowToDouble(row[6]);;
        spacecraftInit.targetVelZ = parser.convertRowToDouble(row[7]);;
        spacecraftInit.targetAccX = parser.convertRowToDouble(row[8]);;
        spacecraftInit.targetAccY = parser.convertRowToDouble(row[9]);;
        spacecraftInit.targetAccZ = parser.convertRowToDouble(row[10]);;

        // Add the init object
        _spacecraftInitData.push_back(spacecraftInit);
    }

    // Return true to indicate that the file was loaded successfully
    return true;
}


/*
// Default values
    auto default_vector = VecDouble(0, 0, 0);

    // Initialize the System
    System* system = new System("Pok'Tul Zar");

    // Initalize the Planets
    auto planet1 = new Planet("Kaltar Vex", 6700e3, 5.8e24, VecDouble(3e11, 2e11, 0));
    auto planet2 = new Planet("Tha Nal", 7200e3, 6.2e24, VecDouble(-2e11, -5e11, 0));
    auto planet3 = new Planet("Tyor Lom", 6300e3, 5.5e24, VecDouble(-1e11, 4e11, 0));
    auto planet4 = new Planet("Orion", 7000e3, 6.0e24, VecDouble(1e11, 1e11, 0));
    auto planet5 = new Planet("Smeg", 4900e3, 5.9e23, VecDouble(4e11, -3e11, 0));


    planet1->setGravitationalParameter(7.8);
    planet2->setGravitationalParameter(11.2);
    planet3->setGravitationalParameter(9.1);
    planet4->setGravitationalParameter(9);
    planet5->setGravitationalParameter(.40);

    // TODO: Need to check to make sure atmo radius is larger than planet radius
    planet1->setAtmosphereRadius(6800e3);
    planet2->setAtmosphereRadius(7300e3);
    planet3->setAtmosphereRadius(6400e3);
    planet4->setAtmosphereRadius(7100e3);
    planet5->setAtmosphereRadius(5000e3);

    // Add planets to system
    system->addPlanet(planet1);
    system->addPlanet(planet2);
    system->addPlanet(planet3);
    system->addPlanet(planet4);
    system->addPlanet(planet5);

    addSystem(system);

    // Initialize the spacecraft
    Spacecraft* spacecraft = new Spacecraft(this, "Gladiator");

    // Set initial conditions for the spacecraft
    spacecraft->setPosition(default_vector);
    spacecraft->setVelocity(VecDouble(10000000000, 10000000000,0));
    spacecraft->setAcceleration(default_vector);
    spacecraft->setOrientation(default_vector);
    spacecraft->setAngularVelocity(0.1);
    spacecraft->setPlanetInformation(planet5);
    spacecraft->setTargetPlanet(planet1);
    spacecraft->setTargets(planet1->getCenterPosition(), VecDouble(30000000000, 30000000000, 0), VecDouble(100000, 100000, 100000));
    // TODO: Use user interface to choose which planet we start at and where we go?
    // TODO: While looping, listen for key bind press to change values, like thrust or another variables.

    //spacecraft->setMaxVelocity(300000);
    spacecraft->setMaxVelocity(30000000000000000);
    spacecraft->setArea(20);
    spacecraft->setMass(1708500);
    // Set initial conditions for the environment
    //.setAirDensity(1.225);
    //env.setGravitationalParameter(9.8);
    // TODO: add input file to read in information

    // Set control system parameters
    // TODO: get navigation system working with PID controller
    spacecraft->setPID(1.0, 0.01, 0.1);
    spacecraft->setThrust(default_vector);

    addSpacecraft(spacecraft);*/