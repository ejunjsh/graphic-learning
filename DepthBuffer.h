#pragma once

#include <vector>
#include <limits>
#include <cmath>

class DepthBuffer {
public:
    DepthBuffer(int width = 0, int height = 0)
        : width_(width), height_(height) {
        buffer_.assign(static_cast<size_t>(width_) * height_,
                       -std::numeric_limits<double>::infinity());
        enabled_ = true;
    }

    void resize(int width, int height) {
        width_ = width;
        height_ = height;
        buffer_.assign(static_cast<size_t>(width_) * height_,
                       -std::numeric_limits<double>::infinity());
    }

    void clear() {
        std::fill(buffer_.begin(), buffer_.end(),
                  -std::numeric_limits<double>::infinity());
    }

    void setEnabled(bool e) { enabled_ = e; }
    bool enabled() const { return enabled_; }

    // Update using canvas coordinates (0..width-1, 0..height-1)
    // Returns true if the depth buffer was updated (pixel is closer).
    bool updateIfCloserCanvas(int cx, int cy, double inv_z) {
        if (!enabled_ || cx < 0 || cx >= width_ || cy < 0 || cy >= height_) return false;
        size_t idx = static_cast<size_t>(cx) + static_cast<size_t>(cy) * width_;
        if (buffer_[idx] < inv_z) {
            buffer_[idx] = inv_z;
            return true;
        }
        return false;
    }

    // Update using centered coordinates (same mapping as PutPixel in your demo).
    // x,y are in centered coordinate system (origin at canvas center).
    bool updateIfCloserCentered(int x, int y, double inv_z) {
        int canvas_x = width_ / 2 + x;
        int canvas_y = height_ / 2 - y - 1;
        return updateIfCloserCanvas(canvas_x, canvas_y, inv_z);
    }

    // Read depth at canvas coords (returns -inf if out of bounds)
    double atCanvas(int cx, int cy) const {
        if (cx < 0 || cx >= width_ || cy < 0 || cy >= height_) return -std::numeric_limits<double>::infinity();
        return buffer_[static_cast<size_t>(cx) + static_cast<size_t>(cy) * width_];
    }

private:
    int width_{0}, height_{0};
    std::vector<double> buffer_;
    bool enabled_{true};
};