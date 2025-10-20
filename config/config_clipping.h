#ifndef CONFIG_CLIPPING_H
#define CONFIG_CLIPPING_H

#include <cmath>
#include "config/config.h"
#include "color.h"
#include "pointx.h"
#include "vertex.h"
#include "triangle.h"
#include "model.h"
#include "instancex.h"
#include "camera.h"

namespace Clipping
{

    Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT] = {};

    constexpr double viewport_size = 1.0;
    constexpr double projection_plane_z = 1.0;

    // Initialize all pixels to white
    inline void InitializePixelsWhite()
    {
        for (int x = 0; x < CANVAS_WIDTH; ++x)
        {
            for (int y = 0; y < CANVAS_HEIGHT; ++y)
            {
                pixels[x][y] = Color(255, 255, 255);
            }
        }
    }

    void PutPixel(double x, double y, const Color &color)
    {
        int canvas_x = CANVAS_WIDTH / 2 + static_cast<int>(std::floor(x));
        int canvas_y = CANVAS_HEIGHT / 2 - static_cast<int>(std::floor(y)) - 1;

        if (canvas_x < 0 || canvas_x >= CANVAS_WIDTH || canvas_y < 0 || canvas_y >= CANVAS_HEIGHT)
        {
            return;
        }

        pixels[canvas_x][canvas_y] = color;
    }

    // Converts 2D viewport coordinates to 2D canvas coordinates.
    Point ViewportToCanvas(const Point &p2d)
    {
        return Point(
            p2d.x * CANVAS_WIDTH / viewport_size,
            p2d.y * CANVAS_HEIGHT / viewport_size);
    }

    // Projects a 3D vertex to 2D canvas coordinates.
    Point ProjectVertex(const Vertex &v)
    {
        return ViewportToCanvas(Point(
            v.x * projection_plane_z / v.z,
            v.y * projection_plane_z / v.z));
    }

    // Interpolate returns std::vector<double>
    std::vector<double> Interpolate(int i0, double d0, int i1, double d1)
    {
        if (i0 == i1)
        {
            return {d0};
        }
        std::vector<double> values;
        double a = (d1 - d0) / (i1 - i0);
        double d = d0;
        for (int i = i0; i <= i1; ++i)
        {
            values.push_back(d);
            d += a;
        }
        return values;
    }

    // DrawLine using Point and Color, assumes PutPixel(double x, double y, const Color&)
    void DrawLine(const Point &p0, const Point &p1, const Color &color)
    {
        int x0 = static_cast<int>(std::round(p0.x));
        int y0 = static_cast<int>(std::round(p0.y));
        int x1 = static_cast<int>(std::round(p1.x));
        int y1 = static_cast<int>(std::round(p1.y));
        int dx = x1 - x0, dy = y1 - y0;

        if (std::abs(dx) > std::abs(dy))
        {
            if (dx < 0)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            auto ys = Interpolate(x0, y0, x1, y1);
            for (int x = x0; x <= x1; ++x)
            {
                PutPixel(x, ys[x - x0], color);
            }
        }
        else
        {
            if (dy < 0)
            {
                std::swap(x0, x1);
                std::swap(y0, y1);
            }
            auto xs = Interpolate(y0, x0, y1, x1);
            for (int y = y0; y <= y1; ++y)
            {
                PutPixel(xs[y - y0], y, color);
            }
        }
    }

    void DrawWirefameTriangle(const Point &p0, const Point &p1, const Point &p2, const Color &color)
    {
        DrawLine(p0, p1, color);
        DrawLine(p1, p2, color);
        DrawLine(p2, p0, color);
    }

    inline Vertex IntersectPlane(const Plane &plane, const Vertex &v1, const Vertex &v2)
    {
        // p(t) = v1 + t*(v2-v1), find t so Dot(n, p(t)) + d = 0
        Vertex dir{v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
        double denom = Dot(plane.normal, dir);
        double numer = -(Dot(plane.normal, v1) + plane.distance);
        double t = denom == 0.0 ? 0.0 : numer / denom;
        return Vertex(v1.x + dir.x * t, v1.y + dir.y * t, v1.z + dir.z * t);
    }

    inline void ClipTriangle(const Triangle &triangle,
                             const Plane &plane,
                             std::vector<Triangle> &out_triangles,
                             std::vector<Vertex> &vertexes)
    {
        Vertex v0 = vertexes[triangle.v0];
        Vertex v1 = vertexes[triangle.v1];
        Vertex v2 = vertexes[triangle.v2];

        bool in0 = Dot(plane.normal, v0) + plane.distance > 0.0;
        bool in1 = Dot(plane.normal, v1) + plane.distance > 0.0;
        bool in2 = Dot(plane.normal, v2) + plane.distance > 0.0;

        int in_count = static_cast<int>(in0) + static_cast<int>(in1) + static_cast<int>(in2);

        if (in_count == 0)
        {
            // fully clipped out
            return;
        }

        if (in_count == 3)
        {
            // fully inside
            out_triangles.push_back(triangle);
            return;
        }

        if (in_count == 1)
        {
            // one vertex inside -> one output triangle
            int in_idx, out_idx0, out_idx1;
            if (in0)
            {
                in_idx = triangle.v0;
                out_idx0 = triangle.v1;
                out_idx1 = triangle.v2;
            }
            else if (in1)
            {
                in_idx = triangle.v1;
                out_idx0 = triangle.v2;
                out_idx1 = triangle.v0;
            }
            else
            {
                in_idx = triangle.v2;
                out_idx0 = triangle.v0;
                out_idx1 = triangle.v1;
            }

            Vertex i0 = IntersectPlane(plane, vertexes[in_idx], vertexes[out_idx0]);
            Vertex i1 = IntersectPlane(plane, vertexes[in_idx], vertexes[out_idx1]);

            int idx_i0 = static_cast<int>(vertexes.size());
            vertexes.push_back(i0);
            int idx_i1 = static_cast<int>(vertexes.size());
            vertexes.push_back(i1);

            out_triangles.emplace_back(in_idx, idx_i0, idx_i1, triangle.color);
            return;
        }

        // in_count == 2 : two vertices inside -> two output triangles
        int out_idx, in_idx0, in_idx1;
        if (!in0)
        {
            out_idx = triangle.v0;
            in_idx0 = triangle.v1;
            in_idx1 = triangle.v2;
        }
        else if (!in1)
        {
            out_idx = triangle.v1;
            in_idx0 = triangle.v2;
            in_idx1 = triangle.v0;
        }
        else
        {
            out_idx = triangle.v2;
            in_idx0 = triangle.v0;
            in_idx1 = triangle.v1;
        }

        Vertex i0 = IntersectPlane(plane, vertexes[in_idx0], vertexes[out_idx]);
        Vertex i1 = IntersectPlane(plane, vertexes[in_idx1], vertexes[out_idx]);

        int idx_i0 = static_cast<int>(vertexes.size());
        vertexes.push_back(i0);
        int idx_i1 = static_cast<int>(vertexes.size());
        vertexes.push_back(i1);

        // two triangles: (in0, in1, i0) and (in1, i1, i0)
        out_triangles.emplace_back(in_idx0, in_idx1, idx_i0, triangle.color);
        out_triangles.emplace_back(in_idx1, idx_i1, idx_i0, triangle.color);
    }

    inline std::optional<Model> TransformAndClip(const std::vector<Plane> &clipping_planes,
                                                 const Model &model,
                                                 const Mat4x4 &transform)
    {
        // Transform bounding sphere center (homogeneous multiply)
        Vertex4 center4 = transform * Vertex4(model.bounds_center);
        Vertex center;
        if (center4.w != 0.0)
        {
            center = Vertex(center4.x / center4.w,
                            center4.y / center4.w,
                            center4.z / center4.w);
        }
        else
        {
            center = Vertex(center4.x, center4.y, center4.z);
        }

        // Early reject against clipping planes using bounding sphere
        for (const Plane &p : clipping_planes)
        {
            double distance = Dot(p.normal, center) + p.distance;
            if (distance < -model.bounds_radius)
            {
                return std::nullopt; // fully culled
            }
        }

        // Apply modelview transform to all vertices (convert to 3D by /w)
        std::vector<Vertex> vertexes;
        vertexes.reserve(model.vertexes.size());
        for (const Vertex &v : model.vertexes)
        {
            Vertex4 tv4 = transform * Vertex4(v);
            if (tv4.w != 0.0)
            {
                vertexes.emplace_back(tv4.x / tv4.w, tv4.y / tv4.w, tv4.z / tv4.w);
            }
            else
            {
                vertexes.emplace_back(tv4.x, tv4.y, tv4.z);
            }
        }

        // Clip the model triangles against each plane in sequence
        std::vector<Triangle> triangles = model.triangles;
        for (const Plane &p : clipping_planes)
        {
            std::vector<Triangle> new_triangles;
            new_triangles.reserve(triangles.size());
            for (const Triangle &t : triangles)
            {
                ClipTriangle(t, p, new_triangles, vertexes);
            }
            triangles.swap(new_triangles);
            if (triangles.empty())
                break; // nothing left to render
        }

        // Build and return clipped model (assumes Model has matching constructor)
        Model out_model(vertexes, triangles, center, model.bounds_radius);
        return out_model;
    }

    void RenderTriangle(const Triangle &triangle, const std::vector<Point> &projected)
    {
        DrawWirefameTriangle(projected[triangle.v0], projected[triangle.v1], projected[triangle.v2], triangle.color);
    }

    // Renders a model
    void RenderModel(const Model &model)
    {
        std::vector<Point> projected;
        projected.reserve(model.vertexes.size());
        for (const auto &v : model.vertexes)
        {
            projected.push_back(ProjectVertex(v));
        }
        for (const auto &tri : model.triangles)
        {
            RenderTriangle(tri, projected);
        }
    }

    void RenderScene(const Camera &camera, const std::vector<Instance> &instances)
    {
        Mat4x4 cameraMatrix = camera.orientation.Transposed() *
                              MakeTranslationMatrix(-camera.position);

        for (const auto &instance : instances)
        {
            Mat4x4 transform = cameraMatrix * instance.transform;

            // Transform the model by transform, clip against camera planes.
            auto clipped_model_opt = TransformAndClip(camera.clipping_planes, instance.model, transform);
            if (!clipped_model_opt)
                continue;

            RenderModel(*clipped_model_opt);
        }
    }

    auto Identity4x4 = Mat4x4{
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}};

}

#endif // CONFIG_CLIPPING_H