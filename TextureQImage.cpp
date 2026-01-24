#include "TextureQImage.h"
#include <algorithm>

bool TextureQImage::load(const QString &path)
{
    QImage loaded(path);
    if (loaded.isNull()) return false;

    // Convert to a predictable format. If your Qt is older, use Format_ARGB32.
#ifdef QIMAGE_FORMAT_RGBA8888
    img = loaded.convertToFormat(QImage::Format_RGBA8888);
#else
    img = loaded.convertToFormat(QImage::Format_ARGB32);
#endif

    return true;
}

QColor TextureQImage::sample(float u, float v) const
{
    if (img.isNull()) return QColor(0,0,0);

    // Clamp u,v to [0,1]. You can change to wrap by using fmod instead.
    u = std::min(std::max(u, 0.0f), 1.0f);
    v = std::min(std::max(v, 0.0f), 1.0f);

    // Map to pixel coordinates. Use width-1/height-1 to avoid sampling off-by-one.
    int x = static_cast<int>(u * (img.width()  - 1) + 0.5f);
    int y = static_cast<int>(v * (img.height() - 1) + 0.5f);

    return img.pixelColor(x, y);
}