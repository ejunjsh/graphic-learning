#pragma once

#include "color.h"

class Triangle {
public:
    int v0, v1, v2; // Vertices of the triangle
    Color color;       // Color of the triangle


    // Constructor
    Triangle(int v0, int v1, int v2, const Color& color)
        : v0(v0), v1(v1), v2(v2), color(color) {}
};