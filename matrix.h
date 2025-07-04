// matrix.h
#pragma once

#include "vector.h"

class Matrix3x3 {
public:
    double m[3][3];

    Matrix3x3() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = 0.0;
    }

    Matrix3x3(double mat[3][3]) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                m[i][j] = mat[i][j];
    }

    // Multiply matrix by vector
    Vector operator*(const Vector& v) const {
        return Vector(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
        );
    }
};