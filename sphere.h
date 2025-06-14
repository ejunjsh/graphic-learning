#ifndef SPHERE_H
#define SPHERE_H

#include "vector.h" // For the center field
#include "color.h"  // For the color field

class Sphere {
public:
    Vector center; // Center of the sphere
    int radius;    // Radius of the sphere
    Color color;   // Color of the sphere
    int specular; // Specular reflection of the sphere

    // Constructor
    Sphere(const Vector& center, int radius, const Color& color)
        : center(center), radius(radius), color(color) {}

    // Constructor with specular reflection
    Sphere(const Vector& center, int radius, const Color& color, int specular)
        : center(center), radius(radius), color(color), specular(specular) {}
};

#endif // SPHERE_H
