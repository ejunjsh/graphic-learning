#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class Model {
public:
    std::vector<Vertex> vertexes;
    std::vector<Triangle> triangles;
    Vertex bounds_center; // geometric center of the model
    double bounds_radius; // radius of the bounding sphere

    Model(const std::vector<Vertex>& vertexes, const std::vector<Triangle>& triangles)
        : vertexes(vertexes), triangles(triangles) {}

    Model(const std::vector<Vertex>& vertexes, const std::vector<Triangle>& triangles,
          const Vertex& bounds_center, double bounds_radius)
        : vertexes(vertexes), triangles(triangles),
          bounds_center(bounds_center), bounds_radius(bounds_radius) {}
};

#endif // MODEL_H