#ifndef CONFIG_H
#define CONFIG_H

#include "vector.h"
#include "color.h"
#include "sphere.h"

const int CANVAS_WIDTH = 600;
const int CANVAS_HEIGHT = 600;

const int VIEWPORT_SIZE = 1;
const int PROJECT_PLANE_Z = 1;
const Vector CAMERA_POSITION(0, 0, 0);
const Color BACKGROUND_COLOR(255, 255, 255);
const Sphere SPHERES[] = {
    Sphere(Vector(0, -1, 3), 1, Color(255, 0, 0)),   // Example sphere 1
    Sphere(Vector(2, 0, 4), 1, Color(0, 0, 255)),    // Example sphere 2
    Sphere(Vector(-2, 0, 4), 1, Color(0, 255, 0))    // Example sphere 3
};

const int NUM_SPHERES = sizeof(SPHERES) / sizeof(SPHERES[0]);

// Converts 2D canvas coordinates to 3D viewport coordinates
inline Vector CanvasToViewport(int x, int y) {
    return Vector(
        x * VIEWPORT_SIZE / static_cast<double>(CANVAS_WIDTH),
        y * VIEWPORT_SIZE / static_cast<double>(CANVAS_HEIGHT),
        PROJECT_PLANE_Z
    );
}

// Computes the intersection of a ray and a sphere.
// Returns true if there is an intersection, and sets t1 and t2.
inline bool IntersectRaySphere(const Vector& origin, const Vector& direction, const Sphere& sphere, double& t1, double& t2) {
    Vector oc = origin - sphere.center;

    double k1 = direction.Dot(direction);
    double k2 = 2 * oc.Dot(direction);
    double k3 = oc.Dot(oc) - sphere.radius * sphere.radius;

    double discriminant = k2 * k2 - 4 * k1 * k3;
    if (discriminant < 0) {
        t1 = t2 = std::numeric_limits<double>::infinity();
        return false;
    }

    t1 = (-k2 + std::sqrt(discriminant)) / (2 * k1);
    t2 = (-k2 - std::sqrt(discriminant)) / (2 * k1);
    return true;
}

// Returns the color of the closest sphere intersected by the ray, or background color.
inline Color TraceRay(const Vector& origin, const Vector& direction, double min_t, double max_t) {
    double closest_t = std::numeric_limits<double>::infinity();
    const Sphere* closest_sphere = nullptr;

    for (int i = 0; i < NUM_SPHERES; ++i) {
        double t1, t2;
        if (IntersectRaySphere(origin, direction, SPHERES[i], t1, t2)) {
            if (t1 < closest_t && min_t < t1 && t1 < max_t) {
                closest_t = t1;
                closest_sphere = &SPHERES[i];
            }
            if (t2 < closest_t && min_t < t2 && t2 < max_t) {
                closest_t = t2;
                closest_sphere = &SPHERES[i];
            }
        }
    }

    if (closest_sphere == nullptr) {
        return BACKGROUND_COLOR;
    }

    return closest_sphere->color;
}

#endif // CONFIG_H