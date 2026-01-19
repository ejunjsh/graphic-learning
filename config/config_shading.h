#ifndef CONFIG_SHADING_H
#define CONFIG_SHADING_H


// Lighting model flags (used by demos / HTML counterparts)
#define LM_DIFFUSE  1
#define LM_SPECULAR 2

// Shading model identifiers
#define SM_FLAT    0
#define SM_GOURAUD 1
#define SM_PHONG   2


#include <cmath>
#include "config/config.h"
#include "color.h"
#include "pointx.h"
#include "vertex.h"
#include "triangle.h"
#include "model.h"
#include "instancex.h"
#include "camera.h"
#include "light.h"
#include "DepthBuffer.h"
#include "GenerateSphere.h"

namespace Shading
{

    Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT] = {};

    constexpr double viewport_size = 1.0;
    constexpr double projection_plane_z = 1.0;

    DepthBuffer depthBuffer(CANVAS_WIDTH, CANVAS_HEIGHT);

    // Current lighting/shading settings (defaults mirror cgfs/shading-demo)
    int LightingModel = LM_DIFFUSE | LM_SPECULAR; // bitmask: LM_DIFFUSE | LM_SPECULAR
    int ShadingModel = SM_PHONG; // SM_FLAT | SM_GOURAUD | SM_PHONG
    bool UseVertexNormals = true; // when true, use per-vertex normals from model

    // Initialize all pixels to white
    inline void InitializePixelsWhite()
    {
        // Clear depth buffer
        depthBuffer.clear();

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
        int canvas_x = CANVAS_WIDTH / 2 + static_cast<int>(x);
        int canvas_y = CANVAS_HEIGHT / 2 - static_cast<int>(y) - 1;

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

    // Converts 2D canvas coordinates to 2D viewport coordinates.
    Point CanvasToViewport(const Point &p2d)
    {
        return Point(
            p2d.x * viewport_size / CANVAS_WIDTH,
            p2d.y * viewport_size / CANVAS_HEIGHT);
    }

    // Un-project a canvas pixel (x,y) with given inverse-Z back into camera-space Vertex.
    // x,y should be canvas coordinates (centered, same convention as PutPixel).
    inline Vertex UnProjectVertex(double x, double y, double inv_z)
    {
        double oz = 1.0 / inv_z;
        double ux = x * oz / projection_plane_z;
        double uy = y * oz / projection_plane_z;
        Point p2d = CanvasToViewport(Point(ux, uy));
        return Vertex(p2d.x, p2d.y, oz);
    }

    // Projects a 3D vertex to 2D canvas coordinates.
    Point ProjectVertex(const Vertex &v)
    {
        return ViewportToCanvas(Point(
            v.x * projection_plane_z / v.z,
            v.y * projection_plane_z / v.z));
    }

    // Return the three local vertex indexes (0,1,2) sorted by projected Y (bottom -> top).
    // vertex_indexes: array of 3 global vertex indices for the triangle (not modified).
    // projected: vector of projected 2D points; each Point must have .y member.
    inline std::array<int,3> SortedVertexIndexes(const std::array<int,3>& vertex_indexes,
                                                const std::vector<Point>& projected)
    {
        std::array<int,3> idx = {0, 1, 2};

        if (projected[vertex_indexes[idx[1]]].y < projected[vertex_indexes[idx[0]]].y)
            std::swap(idx[0], idx[1]);
        if (projected[vertex_indexes[idx[2]]].y < projected[vertex_indexes[idx[0]]].y)
            std::swap(idx[0], idx[2]);
        if (projected[vertex_indexes[idx[2]]].y < projected[vertex_indexes[idx[1]]].y)
            std::swap(idx[1], idx[2]);

        return idx;
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
            if (i == i1) {
                values.push_back(d1); 
                break;
            }
            values.push_back(d);
            d += a;
        }
        return values;
    }

    inline std::pair<std::vector<double>, std::vector<double>>
    EdgeInterpolate(int y0, double v0, int y1, double v1, int y2, double v2) 
    {
        auto v01 = Interpolate(y0, v0, y1, v1);
        auto v12 = Interpolate(y1, v1, y2, v2);
        auto v02 = Interpolate(y0, v0, y2, v2);

        if (!v01.empty()) v01.pop_back(); // remove duplicate at y1
        std::vector<double> v012;
        v012.reserve(v01.size() + v12.size());
        v012.insert(v012.end(), v01.begin(), v01.end());
        v012.insert(v012.end(), v12.begin(), v12.end());

        return { std::move(v02), std::move(v012) };
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

    inline Vertex IntersectPlane(const Plane &plane, const Vertex &v1, const Vertex &v2)
    {
        // p(t) = v1 + t*(v2-v1), find t so Dot(n, p(t)) + d = 0
        Vertex dir{v2.x - v1.x, v2.y - v1.y, v2.z - v1.z};
        double denom = Dot(plane.normal, dir);
        double numer = -(Dot(plane.normal, v1) + plane.distance);
        double t = denom == 0.0 ? 0.0 : numer / denom;
        return Vertex(v1.x + dir.x * t, v1.y + dir.y * t, v1.z + dir.z * t);
    }

    // Compute illumination at a point (vertex) given a normal, camera and lights.
    inline double ComputeIllumination(const Vertex &vertex, const Vertex &normal, const Camera &camera, const std::vector<Light> &lights)
    {
        double illumination = 0.0;

        for (const Light &light : lights)
        {
            if (light.type == Light::AMBIENT)
            {
                illumination += light.intensity;
                continue;
            }

            // vl will be the vector from vertex to light (in camera space)
            Vertex vl;
            if (light.type == Light::DIRECTIONAL)
            {
                // Rotate directional light by camera orientation (transpose)
                Mat4x4 camMat = camera.orientation.Transposed();
                Vertex4 rotated = camMat * Vertex4(Vertex(light.position.x, light.position.y, light.position.z));
                vl = Vertex(rotated.x, rotated.y, rotated.z);
            }
            else // POINT
            {
                Mat4x4 camMat = camera.orientation.Transposed() * MakeTranslationMatrix(Vertex(-camera.position.x, -camera.position.y, -camera.position.z));
                Vertex4 transformed = camMat * Vertex4(Vertex(light.position.x, light.position.y, light.position.z));
                Vertex tlight(transformed.x, transformed.y, transformed.z);
                vl = Vertex(tlight.x - vertex.x, tlight.y - vertex.y, tlight.z - vertex.z);
            }

            double mag_vl = std::sqrt(Dot(vl, vl));
            double mag_n = std::sqrt(Dot(normal, normal));

            // Diffuse
            if ((LightingModel & LM_DIFFUSE) && mag_vl > 0.0 && mag_n > 0.0)
            {
                double cos_alpha = Dot(vl, normal) / (mag_vl * mag_n);
                if (cos_alpha > 0.0)
                    illumination += cos_alpha * light.intensity;
            }

            // Specular
            if (LightingModel & LM_SPECULAR)
            {
                // reflected = 2*(n·vl)*n - vl
                double ndotv = Dot(normal, vl);
                Vertex reflected(
                    2.0 * ndotv * normal.x - vl.x,
                    2.0 * ndotv * normal.y - vl.y,
                    2.0 * ndotv * normal.z - vl.z);

                Vertex view = Vertex(camera.position.x - vertex.x,
                                     camera.position.y - vertex.y,
                                     camera.position.z - vertex.z);

                double mag_ref = std::sqrt(Dot(reflected, reflected));
                double mag_view = std::sqrt(Dot(view, view));
                if (mag_ref > 0.0 && mag_view > 0.0)
                {
                    double cos_beta = Dot(reflected, view) / (mag_ref * mag_view);
                    if (cos_beta > 0.0)
                    {
                        const double specularExp = 50.0;
                        illumination += std::pow(cos_beta, specularExp) * light.intensity;
                    }
                }
            }
        }

        return illumination;
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


inline void RenderTriangle(const Triangle& triangle,
                           const std::vector<Vertex>& vertexes,
                           const std::vector<Point>& projected,
                           const Camera& camera,
                           const std::vector<Light>& lights,
                           const Mat4x4& orientation)
{
    // build local vertex index array (triangle.v0/v1/v2)
    std::array<int,3> verts = { triangle.v0, triangle.v1, triangle.v2 };

    // sort local indexes by projected Y (bottom -> top)
    std::array<int,3> order = SortedVertexIndexes(verts, projected);
    int i0 = order[0], i1 = order[1], i2 = order[2];
    auto idx = [&](int local)->int { return verts[local]; };

    // original vertices for normal / centroid
    const Vertex &A = vertexes[triangle.v0];
    const Vertex &B = vertexes[triangle.v1];
    const Vertex &C = vertexes[triangle.v2];

    // backface culling: use triangle normal (unsorted vertex order)
    Vertex normal = ComputeTriangleNormal(A, B, C);
    Vertex centre{ (A.x + B.x + C.x) / -3.0,
                   (A.y + B.y + C.y) / -3.0,
                   (A.z + B.z + C.z) / -3.0 };
    auto dot_product = Dot(centre, normal);
    if (dot_product < 0.0) return; // cull

    // projected points (sorted)
    Point p0 = projected[idx(i0)];
    Point p1 = projected[idx(i1)];
    Point p2 = projected[idx(i2)];

    int y0 = static_cast<int>(std::round(p0.y));
    int y1 = static_cast<int>(std::round(p1.y));
    int y2 = static_cast<int>(std::round(p2.y));

    // edge interpolation for X and 1/Z
    auto xe = EdgeInterpolate(y0, p0.x, y1, p1.x, y2, p2.x);
    auto ze = EdgeInterpolate(y0, 1.0 / vertexes[idx(i0)].z,
                                   y1, 1.0 / vertexes[idx(i1)].z,
                                   y2, 1.0 / vertexes[idx(i2)].z);

    const std::vector<double> &x02  = xe.first;
    const std::vector<double> &x012 = xe.second;
    const std::vector<double> &iz02 = ze.first;
    const std::vector<double> &iz012= ze.second;

    // pick left/right by comparing midpoint
    std::size_t m = x02.size() / 2;
    const std::vector<double> *x_left, *x_right, *iz_left, *iz_right;
    if (m < x02.size() && m < x012.size() && x02[m] < x012[m]) {
        x_left = &x02;  x_right = &x012;
        iz_left  = &iz02; iz_right = &iz012;
    } else {
        x_left = &x012; x_right = &x02;
        iz_left  = &iz012; iz_right = &iz02;
    }

    // rasterize scanlines
    for (int y = y0; y <= y2; ++y) {
        int row = y - y0;
        if (row < 0) continue;
        if (static_cast<std::size_t>(row) >= x_left->size() ||
            static_cast<std::size_t>(row) >= x_right->size() ||
            static_cast<std::size_t>(row) >= iz_left->size() ||
            static_cast<std::size_t>(row) >= iz_right->size()) continue;

        int xl = static_cast<int>(std::round((*x_left)[row]));
        int xr = static_cast<int>(std::round((*x_right)[row]));
        if (xr < xl) std::swap(xl, xr);

        // interpolate inverse-z across scanline
        auto zscan = Interpolate(xl, (*iz_left)[row], xr, (*iz_right)[row]);

        for (int x = xl; x <= xr; ++x) {
            double inv_z = zscan[x - xl];
            if (depthBuffer.updateIfCloserCentered(x, y, inv_z)) {
                PutPixel(x, y, triangle.color);
            }
        }
    }
}

    // Renders a model with camera, lights and instance orientation
    void RenderModel(const Model &model, const Camera &camera, const std::vector<Light> &lights, const Mat4x4 &orientation)
    {
        std::vector<Point> projected;
        projected.reserve(model.vertexes.size());
        
        for (const auto &v : model.vertexes)
        {
            projected.push_back(ProjectVertex(v));
        }
        for (const auto &tri : model.triangles)
        {
            RenderTriangle(tri, model.vertexes, projected, camera, lights, orientation);
        }
    }

        void RenderScene(const Camera &camera, const std::vector<Instance> &instances, const std::vector<Light> &lights)
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

            RenderModel(*clipped_model_opt, camera, lights, instance.orientation);
        }
    }

    auto Identity4x4 = Mat4x4{
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}};

    auto vertexes = std::vector<Vertex>{
            Vertex{1, 1, 1}, Vertex{-1, 1, 1}, Vertex{-1, -1, 1}, Vertex{1, -1, 1},
            Vertex{1, 1, -1}, Vertex{-1, 1, -1}, Vertex{-1, -1, -1}, Vertex{1, -1, -1}
    };

    auto RED = Color(255, 0, 0);
    auto GREEN = Color(0, 255, 0);
    auto BLUE = Color(0, 0, 255);
    auto YELLOW = Color(255, 255, 0);
    auto PURPLE = Color(255, 0, 255);
    auto CYAN = Color(0, 255, 255);

        auto triangles = std::vector<Triangle>{
            // front face (normal +Z)
            Triangle{0, 1, 2, RED,    {Vertex{0,0,1}, Vertex{0,0,1}, Vertex{0,0,1}}},
            Triangle{0, 2, 3, RED,    {Vertex{0,0,1}, Vertex{0,0,1}, Vertex{0,0,1}}},
            // back face (normal -Z)
            Triangle{4, 0, 3, GREEN,  {Vertex{1,0,0}, Vertex{1,0,0}, Vertex{1,0,0}}},
            Triangle{4, 3, 7, GREEN,  {Vertex{1,0,0}, Vertex{1,0,0}, Vertex{1,0,0}}},
            // left face (normal +X)
            Triangle{5, 4, 7, BLUE,   {Vertex{0,0,-1}, Vertex{0,0,-1}, Vertex{0,0,-1}}},
            Triangle{5, 7, 6, BLUE,   {Vertex{0,0,-1}, Vertex{0,0,-1}, Vertex{0,0,-1}}},
            // right face (normal -X)
            Triangle{1, 5, 6, YELLOW, {Vertex{-1,0,0}, Vertex{-1,0,0}, Vertex{-1,0,0}}},
            Triangle{1, 6, 2, YELLOW, {Vertex{-1,0,0}, Vertex{-1,0,0}, Vertex{-1,0,0}}},
            // top face (normal +Y)
            Triangle{4, 5, 1, PURPLE, {Vertex{0,1,0}, Vertex{0,1,0}, Vertex{0,1,0}}},
            Triangle{4, 1, 0, PURPLE, {Vertex{0,1,0}, Vertex{0,1,0}, Vertex{0,1,0}}},
            // bottom face (normal -Y)
            Triangle{2, 6, 7, CYAN,   {Vertex{0,-1,0}, Vertex{0,-1,0}, Vertex{0,-1,0}}},
            Triangle{2, 7, 3, CYAN,   {Vertex{0,-1,0}, Vertex{0,-1,0}, Vertex{0,-1,0}}}
        };

    void RenderToPixels() {

        InitializePixelsWhite();

        auto cube = Model(vertexes, triangles, Vertex(0,0,0), std::sqrt(3.0));

        auto sphere = GenerateSphere(15, GREEN);

        auto instances = std::vector<Instance>{
                Instance(cube, Vertex{-1.5, 0, 7}, Identity4x4, 0.75),
                Instance(cube, Vertex{1.25, 2.5, 7.5}, MakeOYRotationMatrix(195)),
                Instance(sphere, Vertex{1.75, -0.5, 7}, Identity4x4, 1.5)
        };

        auto camera = Camera(Vertex{-3, 1, 2}, MakeOYRotationMatrix(-30));

            // Clipping planes (near, left, right, top, bottom) with unit normals
        double s2 = std::sqrt(2.0) / 2.0;
        camera.clipping_planes = {
            Plane(Vertex(0, 0, 1), -1.0),        // Near
            Plane(Vertex(s2, 0, s2), 0.0),       // Left
            Plane(Vertex(-s2, 0, s2), 0.0),      // Right
            Plane(Vertex(0, -s2, s2), 0.0),      // Top
            Plane(Vertex(0, s2, s2), 0.0)        // Bottom
        };

        auto lights = std::vector<Light>{
            Light(Light::AMBIENT, 0.2),
            Light(Light::POINT, 0.6, Vector(2, 1, 0)),
            Light(Light::DIRECTIONAL, 0.2, Vector(1, -4, 4))
        };

        RenderScene(camera, instances, lights);
    }   
}

#endif // CONFIG_SHADING_H