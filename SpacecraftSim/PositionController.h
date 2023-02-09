#ifndef POSITIONCONTROLLER_H
#define POSITIONCONTROLLER_H

#include "Vector3.h"

class PositionController
{
    public:

        PositionController() : _kp(0.1), _ki(0.01), _kd(0.001) {}
        virtual ~PositionController() {}

        void update(double dt, Vector3<double> targetPosition, Vector3<double> currentPosition)
        {
            auto error = targetPosition - currentPosition;

            // Calculate the proportional term for each dimension
            double px = error.x * _kp;
            double py = error.y * _kp;
            double pz = error.z * _kp;

            // Integrate the errors over time for each dimension
            _integral.x += error.x * dt;
            _integral.y += error.y * dt;
            _integral.z += error.z * dt;

            // Calculate the integral terms for each dimension
            double ix = _integral.x * _ki;
            double iy = _integral.y * _ki;
            double iz = _integral.z * _ki;

            // Calculate the derivative terms for each dimension
            double dx = (error.x - _previousError.x) / dt * _kd;
            double dy = (error.y - _previousError.y) / dt * _kd;
            double dz = (error.z - _previousError.z) / dt * _kd;

            _output.x = px + ix + dx;
            _output.y = py + iy + dy;
            _output.z = pz + iz + dz;

            // Store the error for the next iteration
            _previousError = error;
        }

        Vector3<double> getPosition() const { return _position; }
        Vector3<double> getTargetVelocity() const { return _velocity; }
        Vector3<double> getOutput() const { return _output; }

     private:
        double _kp, _ki, _kd;
        Vector3<double> _position, _velocity, _output;
        Vector3<double> _integral, _previousError;
};

#endif // POSITIONCONTROLLER_H