#include "System.h"

#include "Planet.h"

System::System(const std::string& name) : _name(name)
{
}

System::~System()
{
    for (const auto& kv : _planets)
    {
        delete kv.second;
    }
}

void System::addPlanet(Planet* planet)
{
    _planets.emplace(planet->getName(), planet);
}

void System::removePlanet(Planet* planet)
{
    auto i = _planets.find(planet->getName());

    if (i != _planets.end())
    {
        _planets.erase(planet->getName());
    }
}
