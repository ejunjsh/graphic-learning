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

    // Operator -
    Vertex operator-(const Vertex& other) const {
        return Vertex(x - other.x, y - other.y, z - other.z);
    }

        // Dot product as a member
    double Dot(const Vertex& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

inline Vertex operator-(const Vertex& v) {
    return Vertex(-v.x, -v.y, -v.z);
}

// Free-function Dot
inline double Dot(const Vertex& a, const Vertex& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Cross product of two 3D vertices/vectors.
inline Vertex Cross(const Vertex& a, const Vertex& b) {
    return Vertex(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

// Compute triangle normal (v1-v0) x (v2-v0). Not normalized.
inline Vertex ComputeTriangleNormal(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    Vertex v0v1 = v1 - v0;
    Vertex v0v2 = v2 - v0;
    return Cross(v0v1, v0v2);
}