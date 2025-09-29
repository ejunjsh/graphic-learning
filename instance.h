#ifndef INSTANCE_H
#define INSTANCE_H

#include "model.h"
#include "vertex.h"

class Instance {
public:
    Model model;
    Vertex position;

    Instance(const Model& model, const Vertex& position)
        : model(model), position(position) {}
};

#endif // INSTANCE_H