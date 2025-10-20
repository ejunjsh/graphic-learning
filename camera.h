#pragma once

#include "vertex.h"
#include "mat4x4.h"
#include "plane.h"

class Camera {
public:
    Vertex position;
    Mat4x4 orientation;
    std::vector<Plane> clipping_planes;

    Camera(const Vertex& position = Vertex(), const Mat4x4& orientation = Mat4x4())
        : position(position), orientation(orientation) {}

    Camera(const Vertex& position, const Mat4x4& orientation, const std::vector<Plane>& clipping_planes)
        : position(position), orientation(orientation), clipping_planes(clipping_planes) {}
};