#ifndef VECTOR_H
#define VECTOR_H

#include <QtMath>

class Vector {
public:
    double x, y, z;

    // Constructor
    Vector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Length of the vector
    double Length() const {
        return qSqrt(x * x + y * y + z * z);
    }

    // Magnitude of the vector
    double Magnitude() const {
        return Length();
    }

    // Normalize the vector
    Vector Normalize() const {
        double mag = Magnitude();
        return Vector(x / mag, y / mag, z / mag);
    }

    // Addition of two vectors
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y, z + other.z);
    }

    // Subtraction of two vectors
    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y, z - other.z);
    }

    // Dot product of two vectors
    double Dot(const Vector& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    // Cross product of two vectors
    Vector Cross(const Vector& other) const {
        return Vector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
            );
    }

    // Scalar multiplication
    Vector operator*(double scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }
};

#endif // VECTOR_H
