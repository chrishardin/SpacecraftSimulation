#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>
template<typename T>
class Vector3 {
    public:
        T x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3 &v) const {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        Vector3 operator-(const Vector3 &v) const {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        Vector3 operator*(T s) const {
            return Vector3(x * s, y * s, z * s);
        }

        Vector3 operator*(const Vector3 &v) const {
            return Vector3(x * v.x, y * v.y, z * v.z);
        }

        friend Vector3 operator*(T s, const Vector3 &v) {
            return Vector3(v.x * s, v.y * s, v.z * s);
        }

        Vector3 operator/(T s) const {
            return Vector3(x / s, y / s, z / s);
        }

        Vector3 operator[](int i) const {
            switch (i)
            {
            case 0:
                return x;
            case 1:
                return y;
            case 2:
                return z;
            }
        }

        Vector3 operator+=(const Vector3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        // The magnitude function computes the magnitude (or length) of a vector. 
        // It is calculated as the square root of the sum of the squares of the vector's components. 
        T magnitude() const {
            T m = sqrt(x * x + y * y + z * z);

            if (m == 0 || std::isinf(m))
            {
                return 0;
            }
            
            return m;
        }

        // The normalize function returns a unit vector (a vector with magnitude 1) that points in the same direction as the original vector. 
        // To get a unit vector, you divide each component of the vector by its magnitude. 
        // The resulting vector has magnitude 1. This is useful for many applications, such as finding direction vectors or for scaling vector quantities.
        Vector3 normalize() const {
            T m = magnitude();
            if (m == 0 || std::isinf(m)) {
                return Vector3(0, 0, 0);
            }

            return Vector3(x / m, y / m, z / m);
        }

        T dot(const Vector3 &v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        Vector3 cross(const Vector3 &v) const {
            return Vector3(y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x);
        }

        void print() const {
            std::cout << x << ", "<< y << ", " << z << std::endl;
        }
};

#endif // VECTOR3_H
