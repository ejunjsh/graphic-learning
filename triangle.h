#pragma once

#include "color.h"
#include "vertex.h"
#include "texture.h"
#include "pointx.h"
#include <array>
#include <memory>

class Triangle {
public:
    int v0, v1, v2; // Vertices of the triangle
    Color color;       // Color of the triangle
    // Per-vertex normals (one normal per corner). May be all-zero when not provided.
    std::array<Vertex, 3> normals;
    // Texture and UV coordinates
    std::shared_ptr<Texture> texture;
    std::array<Pt, 3> uvs;


    // Constructor
    Triangle(int v0, int v1, int v2, const Color& color, const std::array<Vertex,3>& normals = {}, std::shared_ptr<Texture> texture = nullptr, const std::array<Pt,3>& uvs = {})
        : v0(v0), v1(v1), v2(v2), color(color), normals(normals), texture(texture), uvs(uvs) {}
};