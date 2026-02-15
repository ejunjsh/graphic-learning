#ifndef TEXTURE_H
#define TEXTURE_H

#include <QImage>
#include <QImageReader>
#include <QDebug>
#include "color.h"
#include <QResource>

class Texture {
public:
    Texture(const QString& filePath) {
        // Quick resource existence check (useful for :/ paths)
        QResource res(filePath);
        if (!res.isValid()) {
            qWarning() << "Texture resource not found:" << filePath;
        }

        QImageReader reader(filePath);
        if (!reader.read(&image)) {
            qWarning() << "Texture load failed for" << filePath << ":" << reader.errorString();
        } else {
            image = image.convertToFormat(QImage::Format_RGB888);
            qDebug() << "Texture loaded successfully from path:" << filePath;
            qDebug() << "Texture dimensions:" << image.width() << "x" << image.height();
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