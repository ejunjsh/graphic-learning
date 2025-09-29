#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class Model {
public:
    std::vector<Vertex> vertexes;
    std::vector<Triangle> triangles;

    Model(const std::vector<Vertex>& vertexes, const std::vector<Triangle>& triangles)
        : vertexes(vertexes), triangles(triangles) {}
};

#endif // MODEL_H