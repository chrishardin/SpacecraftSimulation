#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include "PositionController.h"
#include "VelocityController.h"
#include "AccelerationController.h"

#include "Vector3.h"

#include <vector>

class Spacecraft;
class Planet;

struct TargetPlanet
{
    TargetPlanet() {}

    TargetPlanet(std::string _name, Vector3<double> _targetPosition, double _atmosphereRadius) :
        name(_name), targetPosition(_targetPosition), atmosphereRadius(_atmosphereRadius) {}

    ~TargetPlanet() {}

    std::string name;
    Vector3<double> targetPosition;
    double atmosphereRadius;
};


class ControlSystem // : public PositionController, public VelocityController, public AccelerationController
{
    public:
        //ControlSystem() {}
        ControlSystem(Spacecraft* spacecraft);
        virtual ~ControlSystem() {}

        virtual void update(double elapsedTime) = 0;

        void setThrust(const Vector3<double>& t) { _thrust = t; }
        Vector3<double> getThrust() const { return _thrust; }

        void setPID(double kp, double ki, double kd);

        void updatePID(double error);

        void updateGNC();

        void setTargetPlanet(Planet* planet);
        TargetPlanet getTargetPlanet() { return _targetPlanet; }
        
        Vector3<double> getTargetPosition() const { return _targetPlanet.targetPosition; }

        void setTargetVelocity(Vector3<double> v) { _targetVelocity = v; }
        void setTargetAcceleration(Vector3<double> a) { _targetAcceleration = a; }

        Vector3<double> getTargetVelocity() const { return _targetVelocity; }
        Vector3<double> getTargetAcceleration() const { return _targetAcceleration; }

        void applyThrust(Vector3<double> thrust);

        void setPosition(const Vector3<double> &position) { _position = position; }
        void setVelocity(const Vector3<double> &velocity) { _velocity = velocity; }
        void setAcceleration(const Vector3<double> &acceleration) { _acceleration = acceleration; }
        void setOrientation(const Vector3<double>& o) { _orientation = o; }

        virtual Vector3<double> getPosition() const { return _position; }
        virtual Vector3<double> getVelocity() const { return _velocity; }
        virtual Vector3<double> getAcceleration() const { return _acceleration; }
        Vector3<double> getOrientation() const { return _orientation; }

    protected:
        Spacecraft* _spacecraft;
        Vector3<double> _thrust;
        TargetPlanet _targetPlanet;
        Vector3<double> _targetVelocity;
        Vector3<double> _targetAcceleration;

//        double _kp;
//        double _ki;
//        double _kd;
//        double _integral;
//        double _previousError;
//        double _dt;

        Vector3<double> _position;
        Vector3<double> _velocity;
        Vector3<double> _acceleration;
        Vector3<double> _orientation;

        PositionController _positionController;
        VelocityController _velocityController;
        AccelerationController _accelerationController;
};

#endif // CONTROLSYSTEM_H