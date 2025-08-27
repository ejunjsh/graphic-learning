#ifndef CONFIG_TRIANGLE_H
#define CONFIG_TRIANGLE_H

#include <cmath>
#include "config/config.h"
#include "color.h"
#include "point.h"

namespace Triangle
{

    Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT] = {};

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

    // Draws a "broken" line from (x0, y0) to (x1, y1) using simple DDA
    void DrawLineBroken(int x0, int y0, int x1, int y1, const Color &color)
    {
        double a = static_cast<double>(y1 - y0) / (x1 - x0);
        double y = y0;
        for (int x = x0; x <= x1; ++x)
        {
            PutPixel(x, y, color);
            y += a;
        }
    }

    // Interpolate returns std::vector<double>
    std::vector<double> Interpolate(int i0, int d0, int i1, int d1)
    {
        if (i0 == i1)
        {
            return {static_cast<double>(d0)};
        }

        std::vector<double> values;
        double a = static_cast<double>(d1 - d0) / (i1 - i0);
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
        int dx = p1.x - p0.x, dy = p1.y - p0.y;

        if (std::abs(dx) > std::abs(dy))
        {
            // Horizontal-ish
            Point start = p0, end = p1;
            if (dx < 0)
            {
                std::swap(start, end);
            }
            auto ys = Interpolate(start.x, start.y, end.x, end.y);
            for (int x = start.x; x <= end.x; ++x)
            {
                PutPixel(x, ys[x - start.x], color);
            }
        }
        else
        {
            // Vertical-ish
            Point start = p0, end = p1;
            if (dy < 0)
            {
                std::swap(start, end);
            }
            auto xs = Interpolate(start.y, start.x, end.y, end.x);
            for (int y = start.y; y <= end.y; ++y)
            {
                PutPixel(xs[y - start.y], y, color);
            }
        }
    }

    inline void DrawWireframeTriangle(const Point &p0, const Point &p1, const Point &p2, const Color &color)
    {
        DrawLine(p0, p1, color);
        DrawLine(p1, p2, color);
        DrawLine(p0, p2, color);
    }

    inline void DrawFilledTriangle(Point p0, Point p1, Point p2, const Color &color)
    {
        // Sort the points from bottom to top
        if (p1.y < p0.y)
            std::swap(p0, p1);
        if (p2.y < p0.y)
            std::swap(p0, p2);
        if (p2.y < p1.y)
            std::swap(p1, p2);

        // Compute X coordinates of the edges
        auto x01 = Interpolate(p0.y, p0.x, p1.y, p1.x);
        auto x12 = Interpolate(p1.y, p1.x, p2.y, p2.x);
        auto x02 = Interpolate(p0.y, p0.x, p2.y, p2.x);

        // Merge the two short sides
        if (!x01.empty())
            x01.pop_back();
        std::vector<double> x012 = x01;
        x012.insert(x012.end(), x12.begin(), x12.end());

        // Determine which is left and which is right
        std::vector<double> x_left, x_right;
        int m = static_cast<int>(x02.size() / 2);
        if (x02[m] < x012[m])
        {
            x_left = x02;
            x_right = x012;
        }
        else
        {
            x_left = x012;
            x_right = x02;
        }

        // Draw horizontal segments
        for (int y = p0.y; y <= p2.y; ++y)
        {
            int idx = y - p0.y;
            int x_start = static_cast<int>(std::ceil(x_left[idx]));
            int x_end = static_cast<int>(std::floor(x_right[idx]));
            for (int x = x_start; x <= x_end; ++x)
            {
                PutPixel(x, y, color);
            }
        }
    }

}

#endif // CONFIG_TRIANGLE_H