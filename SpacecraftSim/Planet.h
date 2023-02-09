#ifndef PLANET_H
#define PLANET_H

#include "Vector3.h"

#include <cmath>
#include <string>

static double const PI_SHORT = 3.1415926;

class Planet
{
    public:
        Planet();
        Planet(std::string systemName, std::string name, double radius, double mass, Vector3<double> centerPosition);

        bool isLanded(double altitude) const { return altitude < _radius; }

        // Planet get methods
        std::string getSystemName() const { return _systemName; }
        std::string getName() const { return _name; }
        double getRadius() const { return _radius; }
        double getMass() const { return _mass; }
        Vector3<double> getCenterPosition() const { return _centerPosition; }
        Vector3<double> getSurfacePosition() const { return _surfacePosition; }

        // Environment calc methods
        double calcAirDensity(double altitude);
        double calcDragCoefficient(double velocity);
        double calcCrossSectionalArea(double area);

        // Environment get methods
        double GetGravitationalForce();

        // TODO: AirTemp changes by altitude. Cant be constant value
        double GetAirTemperature() { return _airTemperature; };
        double getDragCoefficientParameter() const { return _dragCoefficient; }
        double getGravititationParameter() const { return _gravitationalParameter; }
        double getGravity(double altitude) const { return (_mass * _gravitationalParameter) / pow(altitude + _radius, 2); }
        double getAtmosphereRadius() const { return _atmosphereRadius; }
        double getAtmosphericDensity(double altitude) const { return exp(-altitude / 7e5); }
        double getMaxDragForce(double scArea, double scMaxVelocity, double altitude);
        Vector3<double> getAirResistance(Vector3<double> velocity, double altitude, double area);
        Vector3<double> getGravitationalForce(double altitude, Vector3<double> position) const;
        Vector3<double> getLimitedAirResistance(Vector3<double> velocity, double altitude, double area, double maxAirResistance);
        // TODO: add max air resistance to input file


        // Environment set methods
        void setGravitationalParameter(double gp) { _gravitationalParameter = gp; }
        void setAtmosphereRadius(double ar) { _atmosphereRadius = ar; }

        // Templated methods
        template<typename T>
        Vector3<T> getGravitationalAcceleration(const Vector3<T>& position) const
        {
            T distance = position.magnitude();

            if (distance == 0)
            {
                return Vector3<double>(0, 0, 0);
            }

            return position.normalize() * (-_gravitationalParameter / (distance * distance));
        }

        template<typename T>
        Vector3<T> getDragAcceleration(const Vector3<T>& velocity) const
        {
            double velocityMagnitude = velocity.magnitude();
            return -_dragCoefficient * velocity * velocityMagnitude;
        }


    protected:
        Vector3<double> calcSurfacePosition(double radius);
        
        std::string _systemName;
        std::string _name;
        Vector3<double> _centerPosition;
        Vector3<double> _surfacePosition;
        double _radius;
        double _mass;
        double _gravitationalParameter;
        double _dragCoefficient;
        double _airTemperature;
        double _atmosphereRadius;
};

#endif // PLANET_H