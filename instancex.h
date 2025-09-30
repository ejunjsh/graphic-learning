#ifndef INSTANCE_H
#define INSTANCE_H

#include "model.h"
#include "vertex.h"
#include "mat4x4.h"

class Instance {
public:
    Model model;
    Vertex position;
    Mat4x4 orientation;
    double scale;
    Mat4x4 transform;

    Instance(const Model& model,
             const Vertex& position,
             const Mat4x4& orientation = Mat4x4(),
             double scale = 1.0)
        : model(model), position(position), orientation(orientation), scale(scale)
    {
        // Compose the transform: Translation * (Orientation * Scaling)
        transform = MakeTranslationMatrix(position) *
                    (orientation * MakeScalingMatrix(scale));
    }
};

#endif // INSTANCE_H