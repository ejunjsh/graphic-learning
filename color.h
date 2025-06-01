#ifndef COLOR_H
#define COLOR_H

class Color {
public:
    int r, g, b;

    // Constructor
    Color(int r = 0, int g = 0, int b = 0) : r(r), g(g), b(b) {
        Clamp();
    }

    // Clamp color values to the range [0, 255]
    void Clamp() {
        r = (r < 0) ? 0 : (r > 255) ? 255 : r;
        g = (g < 0) ? 0 : (g > 255) ? 255 : g;
        b = (b < 0) ? 0 : (b > 255) ? 255 : b;
    }

    // Add two colors
    Color operator+(const Color& other) const {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    // Multiply color by a scalar
    Color operator*(float scalar) const {
        return Color(static_cast<int>(r * scalar),
                     static_cast<int>(g * scalar),
                     static_cast<int>(b * scalar));
    }

     Color operator*(const Color& other) const {
        return Color(
            static_cast<int>(r * other.r),
            static_cast<int>(g * other.g),
            static_cast<int>(b * other.b)
        );
    }
};

#endif // COLOR_H
