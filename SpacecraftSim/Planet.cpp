#include "Planet.h"
#include "random_gen.h"

Planet::Planet() {}

Planet::Planet(std::string systemName, std::string name, double radius, double mass, Vector3<double> centerPosition)
        : _systemName(systemName), _name(name), _radius(radius), _mass(mass), _centerPosition(centerPosition), _surfacePosition(calcSurfacePosition(radius))
{
    _dragCoefficient = 2;
    _airTemperature = 70;
}

Vector3<double> Planet::getGravitationalForce(double altitude, Vector3<double> position) const
{
    Vector3<double> direction = _centerPosition - position;
    // TODO: double magnitude = (_mass * getGravititationParameter()) / pow(direction.magnitude(), 2);
    // TODO: return direction.normalize() * magnitude;
    return position;
}


double Planet::getMaxDragForce(double scArea, double scMaxVelocity, double altitude)
{
    auto cross = calcCrossSectionalArea(scArea);
    return 0.5 * calcAirDensity(altitude) * _dragCoefficient * cross * scMaxVelocity * scMaxVelocity;
}

Vector3<double> Planet::getAirResistance(Vector3<double> velocity, double altitude, double area)
{
    double rho = calcAirDensity(altitude);
    double v = velocity.magnitude();
    double Cd = calcDragCoefficient(v);
    double A = calcCrossSectionalArea(area);

    // Calculate air resistance using the following formula
    // F_drag = 0.5 * rho * v^2 * Cd * A
    double airResistance = 0.5 * rho * v * v * Cd * A;
    auto result = velocity.normalize() * (-airResistance);
    return result;
}

// atmospheric model formula to calculate air density based on altitude
double Planet::calcAirDensity(double altitude)
{
    // Uses Interational Standard Atmosphere (ISA) model
    double T0 = 288.15;
    double p0 = 101325.0;
    double L = -0.0065;
    double g = 9.80665;
    double R = 287.05;

    double T = T0 + L * altitude;
    double p = p0 * pow(T / T0, -g / (R * L));

    double result = p / (R * T);

    if (std::isnan(result))
    {
        return 0;
    }

    return result;
}

// formula to calculate drag coefficient based on velocity
double Planet::calcDragCoefficient(double velocity)
{
    double dragCoefficient = 0.5 * pow(velocity, 2);
    return dragCoefficient;
}

// TODO: This doesnt need to be under planet. Is Spacecraft
// formula to calculate cross-sectional area
double Planet::calcCrossSectionalArea(double area)
{
    double crossSectionalArea = PI_SHORT * pow(area, 2);
    return crossSectionalArea;
}

Vector3<double> Planet::calcSurfacePosition(double radius)
{
    // Generate random theta (0 to 2 * pi)
    std::uniform_real_distribution<double> thetaDist(0, 2 * PI_SHORT);
    double theta = thetaDist(random_gen);

    // Generate random phi (0 to pi)
    std::uniform_real_distribution<double> phiDist(0, PI_SHORT);
    double phi = phiDist(random_gen);

    // Calculate x, y, and z coordinates
    double x = radius * sin(phi) * cos(theta);
    double y = radius * sin(phi) * sin(theta);
    double z = radius * cos(phi);

    // Return the surface position as a vector from the planet center
    return Vector3<double>(x, y, z);
}

Vector3<double> Planet::getLimitedAirResistance(Vector3<double> velocity, double altitude, double area, double maxAirResistance)
{
    auto airRes = getAirResistance(velocity, altitude, area);

    if (airRes.magnitude() > maxAirResistance)
    {
        return airRes.normalize() * maxAirResistance;
    }
    return airRes;
}

