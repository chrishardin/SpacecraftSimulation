#include "Spacecraft.h"
#include "random_gen.h"

#include <cmath>
#include <iostream>

Spacecraft::Spacecraft(Scenario* scenario, std::string name) : ControlSystem(this), _scenario(scenario), _associatedPlanet(new Planet()), _name(name), _mass(1000) // ,_planet(env)
{
}

Spacecraft::~Spacecraft()
{
}

void Spacecraft::update(double elapsedTime)
{
    // TODO: We will need to check if we are no longer within planet environment
    
    // Update the control system based on the new state of the spacecraft and the environment 
    ControlSystem::update(elapsedTime);
}


void Spacecraft::setPlanetInformation(Planet* planet)
{
    setAssociatedPlanet(planet);
    calcPositionOnPlanet(planet);
}


void Spacecraft::calcPositionOnPlanet(Planet* planet)
{
    // Calculate a random position on the surface of the planet
    std::uniform_real_distribution<double> dist_theta(-PI_SHORT, PI_SHORT);
    std::uniform_real_distribution<double> dist_phi(0, 2 * PI_SHORT);
    double theta = dist_theta(random_gen);
    double phi = dist_phi(random_gen);

    double x = planet->getRadius() * sin(theta) * cos(phi);
    double y = planet->getRadius() * sin(theta) * sin(phi);
    double z = planet->getRadius() * cos(theta);

    _position = planet->getCenterPosition();
   // _position = planet->getSurfacePosition()// + Vector3<double>(x, y, z);
   // _position = Vector3<double>(x, y, z);
}

/*
void Spacecraft::Update(const Environment& environment, float dt) {
  // Calculate drag force
  float drag_coefficient = 0.5;
  float area = 1.0; // Area of the spacecraft
  Vector3 drag_force = -drag_coefficient * area * environment.density * velocity * velocity.Length();

  // Calculate lift force
  float lift_coefficient = 0.1;
  Vector3 lift_force = lift_coefficient * area * environment.density * velocity.Cross(environment.wind_velocity).Normalized();

  // Calculate net force on the spacecraft
  Vector3 net_force = drag_force + lift_force;
  ApplyForce(net_force);

  // Calculate thermal heating
  float heat_coefficient = 1.0;
  ApplyHeating(heat_coefficient * environment.density * velocity.Length() * velocity.Length() * dt);

  // Update position and velocity
  position += velocity * dt;
  velocity += net_force / environment.mass * dt;

  // Check for atmospheric entry
  if (environment.density > 0) {
    // Perform some action to handle atmospheric entry
  }
}

void Spacecraft::ApplyForce(const Vector3& force) {
  velocity += force;
}

void Spacecraft::ApplyHeating(float heat) {
  this->heat += heat;
}

*/