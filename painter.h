#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include "color.h"
#include "config/config.h"

class Painter : public QWidget {
    Q_OBJECT

public:
    Painter(QWidget *parent = nullptr);

    void render(Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT]);


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Color pixels[CANVAS_WIDTH][CANVAS_HEIGHT]; // 2D array to store pixel colors
};

#endif // PAINTER_H