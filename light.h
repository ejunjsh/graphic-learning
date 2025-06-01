#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light {
public:
    enum LightType {
        AMBIENT,
        POINT,
        DIRECTIONAL
    };

    LightType type;
    double intensity; // Use double instead of float
    QVector3D position; // Only used for POINT and DIRECTIONAL lights

    // Ambient light constructor
    Light(LightType t, double i) : type(t), intensity(i), position(0, 0, 0) {}

    // Point or directional light constructor
    Light(LightType t, double i, const QVector3D& pos) : type(t), intensity(i), position(pos) {}
};

#endif // LIGHT_H