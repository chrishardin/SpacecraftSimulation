#include "ControlSystem.h"

#include "Spacecraft.h"
#include "Scenario.h"
#include "System.h"

#include "PositionController.h"
#include "VelocityController.h"
#include "AccelerationController.h"

ControlSystem::ControlSystem(Spacecraft* spacecraft) : _spacecraft(spacecraft), //_positionController(new PositionController()), _velocityController(new VelocityController()),  _accelerationController(new AccelerationController())
   _position(0.0, 0.0, 0.0), _velocity(0.0, 0.0, 0.0), _acceleration(0.0, 0.0, 0.0), _accelerationController(), _positionController(), _velocityController()
{
}


void ControlSystem::update(double elapsedTime)
{
    // Get current spacecraft information
    auto currentPos = _spacecraft->getPosition();
    auto currentVel = _spacecraft->getVelocity();
    auto currentAccel = _spacecraft->getAcceleration();
    auto currentOrient = _spacecraft->getOrientation();

    updateGNC();

    // Update the position, velocity, and acceleration controllers
   // _accelerationController.update(elapsedTime, _targetAcceleration, _acceleration);
    //setAcceleration(_accelerationController.getAcceleration());

    //setVelocity(getVelocity() + getAcceleration() * elapsedTime);
    //setPosition(getPosition() + getVelocity() * elapsedTime);
    _positionController.update(elapsedTime, getTargetPosition(), _position);
    _velocityController.update(elapsedTime, _positionController.getOutput(), _velocity);
    //_accelerationController.update(elapsedTime, _velocityController.getOutput(), _acceleration);
    auto acceleration = _velocityController.getOutput();
    auto velocity = acceleration * elapsedTime;

    _velocity.x = std::clamp(velocity.x, -_spacecraft->getMaxVelocity(), _spacecraft->getMaxVelocity());
    _velocity.y = std::clamp(velocity.y, -_spacecraft->getMaxVelocity(), _spacecraft->getMaxVelocity());
    _velocity.z = std::clamp(velocity.z, -_spacecraft->getMaxVelocity(), _spacecraft->getMaxVelocity());

    setPosition(getPosition() + getVelocity() * elapsedTime);
    // _spacecraft->applyThrust(_thrust);

    // Update the velocity due to acceleration
   // _spacecraft->setVelocity(currentVel + _spacecraft->getAcceleration() * elapsedTime);

    // Update the position of the spacecraft based on its velocity
   // _spacecraft->setPosition(currentPos + _spacecraft->getVelocity() * elapsedTime);

#if 0
    // Get planet information
    auto planet = _spacecraft->getAssociatedPlanet();

    if (planet)
    {
        // Get air density and cross-sectional area
        double altitude = currentPos.magnitude() - planet->getRadius();
        double airDensity = planet->calcAirDensity(altitude);
        double crossSectionalArea = planet->calcCrossSectionalArea(_spacecraft->getArea());

        // Calculate drag force
        double velMag = currentVel.magnitude();
        double dragCoef = planet->calcDragCoefficient(velMag);
        Vector3<double> dragForce = -0.5 * airDensity * velMag  * velMag * crossSectionalArea * dragCoef * currentVel.normalize();

        // Apply limits on drag force
        double maxDragForce = planet->getMaxDragForce(_spacecraft->getArea(), _spacecraft->getMaxVelocity(), altitude);
        if (dragForce.magnitude() > maxDragForce)
        {
            dragForce = dragForce.normalize() * maxDragForce;
        }

        // Update air resistance
        Vector3<double> airResistanceVector = dragForce / planet->getMass();

        // Handle the case where airResistanceVector is infinity
        if (std::isinf(airResistanceVector.x) || std::isinf(airResistanceVector.y) || std::isinf(airResistanceVector.z))
        {
            std::cout << "INFITITY AIR RESISTANCE!!!" << std::endl;
        }

        // Calculate the orientation-based thrust vector
        Vector3<double> orientationThrust = currentOrient * getThrust();

        // Update the acceleration due to gravity, air resistance, and orientation-based thrust
        Vector3<double> gravitationalAcceleration = planet->getGravitationalAcceleration(currentPos);

        double distanceToCenter = currentPos.magnitude();

        _spacecraft->setAcceleration((orientationThrust - airResistanceVector) / planet->getMass() - gravitationalAcceleration * (currentPos.normalize() / distanceToCenter));
    }
    else
    {
        // TODO: Should thrust still be going if we are in space?
        _spacecraft->setAcceleration(getThrust());
    }

    // Update the velocity due to acceleration
    _spacecraft->setVelocity(currentVel + _spacecraft->getAcceleration() * elapsedTime);

    // Update the position of the spacecraft based on its velocity
    _spacecraft->setPosition(currentPos + _spacecraft->getVelocity() * elapsedTime);

    updateGNC();
#endif
}

// Adding the PID controller
void ControlSystem::setPID(double kp, double ki, double kd) {
    
    /*_kp = kp;
    _ki = ki;
    _kd = kd;*/
}

void ControlSystem::updatePID(double error) {
    /*
    // Calculate the proportional term
    double p = error * _kp;

    // Integrate the error over time
    _integral += error * _dt;

    // Calculate the integral term
    double i = _integral * _ki;

    // Calculate the derivative term
    auto denominator = _dt * _kd;
    double d = 0.0;

    if (denominator != 0.0)
    {
        d = (error - _previousError) / _dt * _kd;
    }

    // Set the thrust based on the PID output
    setThrust(Vector3<double>(p + i + d, 0, 0));

    // Store the error for the next iteration
    _previousError = error;

    */
}


void ControlSystem::updateGNC() 
{
    // Calculate the error between the home position and the spacecraft position
    if (_spacecraft->getAssociatedPlanet())
    {
        // TODO: Eventually use surface position
        Vector3<double> homeError = _spacecraft->getPosition() - _spacecraft->getAssociatedPlanet()->getCenterPosition();

        // Check if the spacecraft is leaving the planet's atmosphere
        // If the distance is greater than the atmosphere radius, disassociate from the planet
        if (homeError.magnitude() > _spacecraft->getAssociatedPlanet()->getAtmosphereRadius())
        {
            // Disassociate the spacecraft from the planet
            _spacecraft->disassociateFromPlanet();
        }

    }
    
    // Calculate the error between the current position and the target position
    Vector3<double> targetError = _spacecraft->getPosition() - getTargetPosition();

    if (targetError.magnitude() == 0)
    {
        std::cout << "targetError = 0. Spacecraft has landed." << std::endl;
        setAcceleration(Vector3<double>(0, 0, 0));
        setVelocity(Vector3<double>(0, 0, 0));
    }
    // Check if the spacecraft is close enough to the target position to start landing
    else if (targetError.magnitude() < getTargetPlanet().atmosphereRadius)
    {
        // Start landing sequence
        // _spacecraft->setLanding(true);

        // Disassociate from the current planet
        _spacecraft->disassociateFromPlanet();

        // Associate with the new planet
        Planet* newPlanet = nullptr;

        for (const auto& kv : _spacecraft->getScenario()->getSystems())
        {
            auto planets = kv.second->getPlanets();

            auto i = planets.find(getTargetPlanet().name);

            if (i != planets.end())
            {
                newPlanet = i->second;
                break;
            }
            else
            {
                std::cerr << "Couldn't find planet name when updating GNC to associate new planet to spacecraft." << std::endl;
            }
        }

        _spacecraft->setAssociatedPlanet(newPlanet);
    }

    // Pass the error to the PID controller
    updatePID(targetError.magnitude());

    // Calculate the desired orientation based on the error
    _spacecraft->setOrientation(targetError.normalize());
}


void ControlSystem::setTargetPlanet(Planet* planet)
{
    // TODO: For now we are using center position. Eventually use calcSurfacePosition()
    auto targetPlanet = TargetPlanet(planet->getName(), planet->getCenterPosition(), 100000.0);
    _targetPlanet = targetPlanet;
}


void ControlSystem::applyThrust(Vector3<double> thrust)
{
    if (_spacecraft->getAssociatedPlanet())
    {
        _acceleration = thrust / _spacecraft->getMass();
    }
    else
    {
        _acceleration = Vector3<double>(0, 0, 0);
    }
};


