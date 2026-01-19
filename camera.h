#pragma once

#include "vertex.h"
#include "mat4x4.h"
#include "plane.h"

class Camera {
public:
    // Camera position in world space.
    Vertex position;

    // Camera orientation as a 4x4 matrix. Use `Transposed()` when building
    // the camera matrix for world->camera transforms (see config_shading usage).
    Mat4x4 orientation;

    // View frustum clipping planes expressed in camera space. Typical order:
    // near, left, right, top, bottom. Used for fast culling of models.
    std::vector<Plane> clipping_planes;

    // Default constructor: optional position and orientation.
    Camera(const Vertex& position = Vertex(), const Mat4x4& orientation = Mat4x4())
        : position(position), orientation(orientation) {}

    // Full constructor: position, orientation, and clipping planes.
    Camera(const Vertex& position, const Mat4x4& orientation, const std::vector<Plane>& clipping_planes)
        : position(position), orientation(orientation), clipping_planes(clipping_planes) {}
};