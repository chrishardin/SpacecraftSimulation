#ifndef SYSTEM_H
#define SYSTEM_H

#include <map>

class Planet;

class System
{
    public:
        //System();
        System(const std::string& name);
        ~System();

        void addPlanet(Planet* planet);
        void removePlanet(Planet* planet);

        std::string getName() const { return _name; }
        std::map<std::string, Planet*>& getPlanets() { return _planets; }

    protected:
        std::string _name;
        std::map<std::string, Planet*> _planets;
        
};

#endif // SYSTEM_H

