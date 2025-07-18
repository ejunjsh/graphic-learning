#ifndef CONFIG_LINES_H
#define CONFIG_LINES_H

#include <cmath>
#include "config/config.h"
#include "color.h"
#include "point.h"

namespace Lines {

    Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT] = {};

    // Initialize all pixels to white
    inline void InitializePixelsWhite() {
        for (int x = 0; x < CANVAS_WIDTH; ++x) {
            for (int y = 0; y < CANVAS_HEIGHT; ++y) {
                pixels[x][y] = Color(255, 255, 255);
            }
        }
    }

    void PutPixel(double x, double y, const Color& color) {
        int canvas_x = CANVAS_WIDTH / 2 + static_cast<int>(std::floor(x));
        int canvas_y = CANVAS_HEIGHT / 2 - static_cast<int>(std::floor(y)) - 1;

        if (canvas_x < 0 || canvas_x >= CANVAS_WIDTH || canvas_y < 0 || canvas_y >= CANVAS_HEIGHT) {
            return;
        }

        pixels[canvas_x][canvas_y] = color;
    }

    // Draws a "broken" line from (x0, y0) to (x1, y1) using simple DDA
    void DrawLineBroken(int x0, int y0, int x1, int y1, const Color& color) {
        double a = static_cast<double>(y1 - y0) / (x1 - x0);
        double y = y0;
        for (int x = x0; x <= x1; ++x) {
            PutPixel(x, y, color);
            y += a;
        }
    }

    // Interpolate returns std::vector<double>
    std::vector<double> Interpolate(int i0, int d0, int i1, int d1) {
        if (i0 == i1) {
            return {static_cast<double>(d0)};
        }

        std::vector<double> values;
        double a = static_cast<double>(d1 - d0) / (i1 - i0);
        double d = d0;
        for (int i = i0; i <= i1; ++i) {
            values.push_back(d);
            d += a;
        }
        return values;
    }

    // DrawLine using Point and Color, assumes PutPixel(double x, double y, const Color&)
    void DrawLine(const Point& p0, const Point& p1, const Color& color) {
        int dx = p1.x - p0.x, dy = p1.y - p0.y;

        if (std::abs(dx) > std::abs(dy)) {
            // Horizontal-ish
            Point start = p0, end = p1;
            if (dx < 0) { std::swap(start, end); }
            auto ys = Interpolate(start.x, start.y, end.x, end.y);
            for (int x = start.x; x <= end.x; ++x) {
                PutPixel(x, ys[x - start.x], color);
            }
        } else {
            // Vertical-ish
            Point start = p0, end = p1;
            if (dy < 0) { std::swap(start, end); }
            auto xs = Interpolate(start.y, start.x, end.y, end.x);
            for (int y = start.y; y <= end.y; ++y) {
                PutPixel(xs[y - start.y], y, color);
            }
        }
    }

}

#endif // CONFIG_LINES_H