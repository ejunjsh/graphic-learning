#pragma once

class Vertex {
public:
    double x, y, z;

    Vertex(double x = 0, double y = 0, double z = 0)
        : x(x), y(y), z(z) {}

    // Operator +
    Vertex operator+(const Vertex& other) const {
        return Vertex(x + other.x, y + other.y, z + other.z);
    }

    Vertex operator-(const Vertex& v) const {
        return Vertex(-v.x, -v.y, -v.z);
    }
};

inline Vertex operator-(const Vertex& v) {
    return Vertex(-v.x, -v.y, -v.z);
}