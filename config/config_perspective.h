#ifndef CONFIG_PERSPECTIVE_H
#define CONFIG_PERSPECTIVE_H

#include <cmath>
#include "config/config.h"
#include "color.h"
#include "pointx.h"
#include "vertex.h"

namespace Perspective
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


}

#endif // CONFIG_PERSPECTIVE_H