// GenerateSphere.h
// Utility to generate a unit sphere mesh as a Model
#ifndef GENERATE_SPHERE_H
#define GENERATE_SPHERE_H

#include <vector>
#include <cmath>
#include "model.h"
#include "vertex.h"
#include "triangle.h"
#include "color.h"

inline Model GenerateSphere(int divs, const Color& color) {
    if (divs < 3) divs = 3;

    std::vector<Vertex> vertexes;
    std::vector<Triangle> triangles;

    const double PI = acos(-1.0);
    double delta_angle = 2.0 * PI / divs;

    // Generate vertex rows from bottom to top (divs+1 rows, each with 'divs' samples)
    for (int d = 0; d <= divs; ++d) {
        double y = (2.0 / divs) * (d - divs / 2.0);
        double radius = std::sqrt(std::max(0.0, 1.0 - y * y));
        for (int i = 0; i < divs; ++i) {
            double x = radius * std::cos(i * delta_angle);
            double z = radius * std::sin(i * delta_angle);
            vertexes.emplace_back(x, y, z);
        }
    }

    // Generate triangles connecting the rows
    for (int d = 0; d < divs; ++d) {
        for (int i = 0; i < divs; ++i) {
            int i0 = d * divs + i;
            int i1 = (d + 1) * divs + (i + 1) % divs;
            int i2 = d * divs + (i + 1) % divs;

            // Triangle 0
            triangles.emplace_back(i0, i1, i2, color);

            // Triangle 1
            triangles.emplace_back(i0, i0 + divs, i1, color);
        }
    }

    return Model(vertexes, triangles, Vertex(0, 0, 0), 1.0);
}

#endif // GENERATE_SPHERE_H
