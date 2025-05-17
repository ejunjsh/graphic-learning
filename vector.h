#ifndef VECTOR_H
#define VECTOR_H

#include <QtMath>

class Vector {
public:
    float x, y, z;

    // Constructor
    Vector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Magnitude of the vector
    float Magnitude() const {
        return qSqrt(x * x + y * y + z * z);
    }

    // Normalize the vector
    Vector Normalize() const {
        float mag = Magnitude();
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
    float Dot(const Vector& other) const {
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
    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar, z * scalar);
    }
};

#endif // VECTOR_H
