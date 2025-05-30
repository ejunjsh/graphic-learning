#include "painter.h"
#include <QPainter>
#include <QDebug>

Painter::Painter(QWidget *parent)
    : QWidget(parent) {
    // Initialization code (if any)
    setFixedSize(600, 600);
}

void Painter::render(Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT]) {
    // Store the pixel colors in the member variable
    for (int x = 0; x < 600; ++x) {
        for (int y = 0; y < 600; ++y) {
            this->pixels[x][y] = pixels[x][y];
        }
    }
    // Request a repaint
    update();
}

void Painter::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // Set the background color to white
    painter.fillRect(rect(), Qt::white);
    // Draw the pixels
    for(int x = 0; x < CANVAS_WIDTH; ++x) {
        for (int y = 0; y < CANVAS_HEIGHT; y++)
        {
            Color pixel = pixels[x][y];
            // Set the color based on the pixel value
            painter.setPen(QColor(pixel.r, pixel.g, pixel.b));
            // Draw the pixel
            painter.drawPoint(x, y);
        }   
    }
}