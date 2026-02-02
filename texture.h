#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>
#include "color.h"

class Texture {
public:
    Texture(const QString& filePath) {
        image.load(filePath);
        if (image.isNull()) {
            // Handle error, maybe throw or log
        }
    }

    Color getTexel(double u, double v) const {
        int iu = static_cast<int>(u * image.width()) % image.width();
        int iv = static_cast<int>(v * image.height()) % image.height();

        if (iu < 0) iu += image.width();
        if (iv < 0) iv += image.height();

        QRgb pixel = image.pixel(iu, iv);
        return Color(qRed(pixel), qGreen(pixel), qBlue(pixel));
    }

private:
    QImage image;
};

#endif // TEXTURE_H