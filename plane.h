#pragma once

#include <cmath>
#include "vertex.h"

// Plane represented as: n · x + d = 0
class Plane {
public:
    Vertex normal;   // plane normal (preferably unit length)
    double distance; // d in plane equation

    Plane(const Vertex& normal = Vertex(0, 1, 0), double distance = 0.0)
        : normal(normal), distance(distance) {}
};