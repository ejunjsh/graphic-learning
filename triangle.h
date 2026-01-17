#pragma once

#include "color.h"
#include "vertex.h"
#include <array>

class Triangle {
public:
    int v0, v1, v2; // Vertices of the triangle
    Color color;       // Color of the triangle
    // Per-vertex normals (one normal per corner). May be all-zero when not provided.
    std::array<Vertex, 3> normals;


    // Constructor
    Triangle(int v0, int v1, int v2, const Color& color, const std::array<Vertex,3>& normals = {})
        : v0(v0), v1(v1), v2(v2), color(color), normals(normals) {}
};