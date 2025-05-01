#include "painter.h"
#include <QPainter>

Painter::Painter(QWidget *parent)
    : QWidget(parent) {
    // Initialization code (if any)
}

void Painter::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // Set the brush and pen
    painter.setBrush(Qt::red);
    painter.setPen(Qt::green);

    // Draw a rectangle
    painter.drawRect(10, 10, 100, 100);

    // Draw some text
    painter.drawText(10, 130, "Hello, Qt!");

    // Draw a line
    painter.drawLine(10, 150, 110, 150);

    // Draw an ellipse
    painter.drawEllipse(10, 160, 100, 50);

    // Draw a point
    painter.drawPoint(0, 220);
}