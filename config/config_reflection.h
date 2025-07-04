#ifndef CONFIG_REFLECTION_H
#define CONFIG_REFLECTION_H

#include "vector.h"
#include "color.h"
#include "sphere.h"
#include "light.h"
#include "config/config.h"

namespace Reflection {

const int VIEWPORT_SIZE = 1;
const int PROJECT_PLANE_Z = 1;
const Vector CAMERA_POSITION(0, 0, 0);
const Color BACKGROUND_COLOR(0, 0, 0);
const Sphere SPHERES[] = {
    Sphere(Vector(0, -1, 3), 1, Color(255, 0, 0), 500, 0.2),
    Sphere(Vector(-2, 0, 4), 1, Color(0, 255, 0), 10, 0.4),
    Sphere(Vector(2, 0, 4), 1, Color(0, 0, 255), 500, 0.3),
    Sphere(Vector(0, -5001, 0), 5000, Color(255, 255, 0), 1000, 0.5)
};

const int NUM_SPHERES = sizeof(SPHERES) / sizeof(SPHERES[0]);

const double EPSILON = 0.001; // Small value to avoid self-intersection

// Converts 2D canvas coordinates to 3D viewport coordinates
inline Vector CanvasToViewport(int x, int y) {
    return Vector(
        x * VIEWPORT_SIZE / static_cast<double>(CANVAS_WIDTH),
        y * VIEWPORT_SIZE / static_cast<double>(CANVAS_HEIGHT),
        PROJECT_PLANE_Z
    );
}

// Computes the reflection of vector v1 with respect to normal v2
inline Vector ReflectRay(const Vector& v1, const Vector& v2) {
    // v1: incident vector, v2: normal (should be normalized)
    return v2 * (2.0 * v1.Dot(v2)) - v1;
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

// Returns a pair: (pointer to closest sphere, closest t). If no intersection, returns {nullptr, infinity}.
inline std::pair<const Sphere*, double> ClosestIntersection(
    const Vector& origin,
    const Vector& direction,
    double min_t,
    double max_t
) {
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

    return {closest_sphere, closest_t};
}

using LightType = Light::LightType;

const Light LIGHTS[] = {
    Light(LightType::AMBIENT, 0.2),
    Light(LightType::POINT, 0.6, Vector(2, 1, 0)),
    Light(LightType::DIRECTIONAL, 0.2, Vector(1, 4, 4))
};
const int NUM_LIGHTS = sizeof(LIGHTS) / sizeof(LIGHTS[0]);

inline double ComputeLighting(const Vector& point, const Vector& normal,  const Vector& view, int specular) {
    double intensity = 0.0;
    double length_n = normal.Length(); // Should be 1.0 if normalized
    double length_v = view.Length();

    for (int i = 0; i < NUM_LIGHTS; ++i) {
        const Light& light = LIGHTS[i];
        if (light.type == LightType::AMBIENT) {
            intensity += light.intensity;
        } else {
            Vector vec_l;
            double t_max;
            if (light.type == LightType::POINT) {
                vec_l = light.position - point;
                t_max = 1.0;
            } else { // DIRECTIONAL
                vec_l = light.position;
                t_max = std::numeric_limits<double>::infinity();

            }

            auto [blocker, t_blocker] = ClosestIntersection(point, vec_l, 0.001, t_max);
            if (blocker != nullptr) {
                continue;
            }
            
            //Diffuse reflection
            double n_dot_l = normal.Dot(vec_l);
            if (n_dot_l > 0) {
                intensity += light.intensity * n_dot_l / (length_n * vec_l.Length());
            }

            // Specular reflection
            if (specular != -1) {
                Vector vec_r = ReflectRay(vec_l, normal);
                double r_dot_v = vec_r.Dot(view);
                if (r_dot_v > 0) {
                    intensity += light.intensity * std::pow(r_dot_v / (vec_r.Length() * length_v), specular);
                }
            }
        }
    }
    return intensity;
}

inline Color TraceRay(const Vector& origin, const Vector& direction, double min_t, double max_t, int depth) {
    auto [closest_sphere, closest_t] = ClosestIntersection(origin, direction, min_t, max_t);
    if (closest_sphere == nullptr) {
        return BACKGROUND_COLOR;
    }

    // Compute intersection point and normal
    Vector point = origin + direction * closest_t;
    Vector normal = point - closest_sphere->center;
    normal = normal * (1.0 / normal.Length()); // Normalize
    Vector view = direction * -1; // View vector is opposite to the ray direction

    // Compute lighting
     double lighting = ComputeLighting(point, normal, view, closest_sphere->specular);
    Color local_color = closest_sphere->color * lighting;

    if (closest_sphere->reflective <= 0 || depth <= 0) {
        return local_color;
    }

    Vector reflected_ray = ReflectRay(view, normal);
    Color reflected_color = TraceRay(point, reflected_ray, EPSILON, std::numeric_limits<double>::infinity(), depth - 1);

    // Combine local and reflected color
    Color result = local_color * (1 - closest_sphere->reflective) +
                   reflected_color * closest_sphere->reflective;

    return result;
}


} // namespace Reflection

#endif // CONFIG_REFLECTION_H