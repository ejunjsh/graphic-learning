#pragma once

#include "vertex.h"

class Vertex4 {
public:
    double x, y, z, w;

    Vertex4(double x = 0, double y = 0, double z = 0, double w = 1)
        : x(x), y(y), z(z), w(w) {}

    // Operator +
    Vertex4 operator+(const Vertex4& other) const {
        return Vertex4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Operator *
    Vertex4 operator*(double scalar) const {
        return Vertex4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vertex4(const Vertex& v)
        : x(v.x), y(v.y), z(v.z), w(1.0) {}
};