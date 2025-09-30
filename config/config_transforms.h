#ifndef CONFIG_TRANSFORMS_H
#define CONFIG_TRANSFORMS_H

#include <cmath>
#include "config/config.h"
#include "color.h"
#include "pointx.h"
#include "vertex.h"
#include "triangle.h"
#include "model.h"
#include "instancex.h"
#include "camera.h"

namespace Transforms
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
    Point ViewportToCanvas(const Point& p2d) {
        return Point(
            p2d.x * CANVAS_WIDTH / viewport_size,
            p2d.y * CANVAS_HEIGHT / viewport_size
        );
    }

    // Projects a 3D vertex to 2D canvas coordinates.
    Point ProjectVertex(const Vertex& v) {
        return ViewportToCanvas(Point(
            v.x * projection_plane_z / v.z,
            v.y * projection_plane_z / v.z
        ));
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

        if (std::abs(dx) > std::abs(dy)) {
            if (dx < 0) { std::swap(x0, x1); std::swap(y0, y1); }
            auto ys = Interpolate(x0, y0, x1, y1);
            for (int x = x0; x <= x1; ++x) {
                PutPixel(x, ys[x - x0], color);
            }
        } else {
            if (dy < 0) { std::swap(x0, x1); std::swap(y0, y1); }
            auto xs = Interpolate(y0, x0, y1, x1);
            for (int y = y0; y <= y1; ++y) {
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

    void RenderTriangle(const Triangle &triangle, const std::vector<Point> &projected)
    {
        DrawWirefameTriangle(projected[triangle.v0], projected[triangle.v1], projected[triangle.v2], triangle.color);
    }

        // Renders a model with the given transformation matrix
    void RenderModel(const Model& model, const Mat4x4& transform) {
        std::vector<Point> projected;
        projected.reserve(model.vertexes.size());
        for (const auto& v : model.vertexes) {
            // Transform vertex to homogeneous coordinates
            Vertex4 vh(v.x, v.y, v.z, 1.0);
            Vertex4 vt = transform * vh;
            // Perspective divide (if needed)
            Vertex v_transformed(vt.x, vt.y, vt.z);
            projected.push_back(ProjectVertex(v_transformed));
        }
        for (const auto& tri : model.triangles) {
            RenderTriangle(tri, projected);
        }
    }
    
    void RenderScene(const Camera& camera, const std::vector<Instance>& instances) {
        Mat4x4 cameraMatrix = camera.orientation.Transposed() *
                            MakeTranslationMatrix(-camera.position);

        for (const auto& instance : instances) {
            Mat4x4 transform = cameraMatrix * instance.transform;
            RenderModel(instance.model, transform);
        }
    }

    auto Identity4x4 = Mat4x4{
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

}

#endif // CONFIG_TRANSFORMS_H