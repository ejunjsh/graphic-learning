#pragma once

#include "vertex.h"
#include "mat4x4.h"

class Camera {
public:
    Vertex position;
    Mat4x4 orientation;

    Camera(const Vertex& position = Vertex(), const Mat4x4& orientation = Mat4x4())
        : position(position), orientation(orientation) {}
};