#ifndef SPACECRAFT_H
#define SPACECRAFT_H

#include "ControlSystem.h"
#include "Planet.h"
#include "Vector3.h"

#include <vector>

class Scenario;

class Spacecraft : public ControlSystem
{
public:
    Spacecraft(Scenario* scenario, std::string name);
    ~Spacecraft();

    //ControlSystem& getControlSystem() { return _controlSystem; }

    Scenario* getScenario() { return _scenario; }

    std::string getName() const { return _name; }

    void update(double elapsedTime);

    void setAngularVelocity(double av) { _angularVelocity = av; }
    const double& getAngularVelocity() { return _angularVelocity; }

    void setMass(double mass) { _mass = mass; }
    //void setDragCoefficient(double dragCoefficient) { _dragCoefficient = dragCoefficient; }
    void setArea(double area) { _area = area; }
    void setMaxVelocity(double maxV) { _maxVelocity = maxV; }

    double getMass() const { return _mass; }
    //double getDragCoefficient() const { return _dragCoefficient; }
    double getArea() const { return _area; }
    double getMaxVelocity() const { return _maxVelocity; }

    void setAssociatedPlanet(Planet* planet) { _associatedPlanet = planet; }
    Planet* getAssociatedPlanet() const { return _associatedPlanet; }

    void disassociateFromPlanet() { _associatedPlanet = nullptr; }


    void setPlanetInformation(Planet* planet);

    // void applyHeating(float heat);
    // void applyForce(const Vector3& force);

    protected:
        void calcPositionOnPlanet(Planet* planet);

        Scenario* _scenario;
        // ControlSystem _controlSystem;
        std::string _name;
        Planet* _associatedPlanet;
        double _angularVelocity;
        double _mass;
        //double _dragCoefficient;
        double _area;
        double _maxVelocity;
};

#endif // SPACECRAFT_H
