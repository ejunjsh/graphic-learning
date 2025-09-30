#pragma once

#include "vertex4.h"

class Mat4x4 {
public:
    double m[4][4];

    Mat4x4() {
        // Initialize to identity matrix
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                m[i][j] = (i == j) ? 1.0 : 0.0;
    }

        // Support initialization with nested initializer lists
    Mat4x4(std::initializer_list<std::initializer_list<double>> list) {
        if (list.size() != 4)
            throw std::invalid_argument("Mat4x4 requires 4 rows");
        int i = 0;
        for (const auto& row : list) {
            if (row.size() != 4)
                throw std::invalid_argument("Mat4x4 requires 4 columns per row");
            int j = 0;
            for (double val : row) {
                m[i][j++] = val;
            }
            ++i;
        }
    }


    // Access operator
    double* operator[](int row) { return m[row]; }
    const double* operator[](int row) const { return m[row]; }

    // Matrix-vector multiplication
    Vertex4 operator*(const Vertex4& v) const {
        return Vertex4(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
            m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w
        );
    }

    // Matrix-matrix multiplication
    Mat4x4 operator*(const Mat4x4& other) const {
        Mat4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0;
                for (int k = 0; k < 4; ++k)
                    result.m[i][j] += m[i][k] * other.m[k][j];
            }
        return result;
    }

    // Returns the transpose of this matrix
    Mat4x4 Transposed() const {
        Mat4x4 result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result.m[i][j] = m[j][i];
        return result;
    }
};


    // Creates a rotation matrix around the Y axis (OY) for the given angle in degrees
    inline Mat4x4 MakeOYRotationMatrix(double degrees) {
        double radians = degrees * M_PI / 180.0;
        double cosA = std::cos(radians);
        double sinA = std::sin(radians);

        return Mat4x4{
            { cosA,  0.0, -sinA, 0.0 },
            { 0.0,   1.0,  0.0,  0.0 },
            { sinA,  0.0,  cosA, 0.0 },
            { 0.0,   0.0,  0.0,  1.0 }
        };
    }

    // Creates a translation matrix for the given translation vector
    inline Mat4x4 MakeTranslationMatrix(const Vertex& translation) {
        return Mat4x4{
            { 1.0, 0.0, 0.0, translation.x },
            { 0.0, 1.0, 0.0, translation.y },
            { 0.0, 0.0, 1.0, translation.z },
            { 0.0, 0.0, 0.0, 1.0 }
        };
    }

    // Creates a scaling matrix for the given scale factor
    inline Mat4x4 MakeScalingMatrix(double scale) {
        return Mat4x4{
            { scale, 0.0,   0.0,   0.0 },
            { 0.0,   scale, 0.0,   0.0 },
            { 0.0,   0.0,   scale, 0.0 },
            { 0.0,   0.0,   0.0,   1.0 }
        };
    }