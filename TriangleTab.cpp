#include "TriangleTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_triangle.h"
#include <QRadioButton>

using namespace Triangle;

TriangleTab::TriangleTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    InitializePixelsWhite();

    Point p0(-200, -250);
    Point p1(200, 50);
    Point p2(20, 250);

    DrawFilledTriangle(p0, p1, p2, Color(0, 255, 0));
    DrawWireframeTriangle(p0, p1, p2, Color(0, 0, 0));

    painter->render(pixels);
}