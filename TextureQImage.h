#pragma once
#include <QString>
#include <QImage>
#include <QColor>
#include <QSize>

class TextureQImage {
public:
    TextureQImage() = default;
    explicit TextureQImage(const QString &path) { load(path); }

    // Loads image from disk. Returns false if load failed.
    bool load(const QString &path);

    // Returns image size
    QSize size() const { return img.size(); }

    // Sample the texture at normalized coordinates (u,v) in [0,1].
    // v=0 -> top row (matches QImage coordinate system).
    // Returns a QColor; use .red(), .green(), .blue().
    QColor sample(float u, float v) const;

    bool isValid() const { return !img.isNull(); }

private:
    QImage img;
};