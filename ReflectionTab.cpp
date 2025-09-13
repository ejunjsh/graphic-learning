#include "ReflectionTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_reflection.h"

using namespace Reflection;

ReflectionTab::ReflectionTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    Painter *painter = new Painter(this);
    layout->addWidget(painter);
    setLayout(layout);

    Color (*pixels)[CANVAS_HEIGHT] = new Color[CANVAS_WIDTH][CANVAS_HEIGHT];

    Vector camera_position = CAMERA_POSITION;
    for (int x = -CANVAS_WIDTH / 2; x < CANVAS_WIDTH / 2; ++x) {
        for (int y = -CANVAS_HEIGHT / 2; y < CANVAS_HEIGHT / 2; ++y) {
            Vector direction = CanvasToViewport(x, y);
            Color color = TraceRay(camera_position, direction, 1.0, std::numeric_limits<double>::infinity(), 3);

            // Convert (x, y) from viewport to canvas coordinates
            int canvas_x = CANVAS_WIDTH / 2 + x;
            int canvas_y = CANVAS_HEIGHT / 2 - y - 1;

            if (canvas_x >= 0 && canvas_x < CANVAS_WIDTH && canvas_y >= 0 && canvas_y < CANVAS_HEIGHT) {
                pixels[canvas_x][canvas_y] = color;
            }
        }
    }

    painter->render(pixels);
}