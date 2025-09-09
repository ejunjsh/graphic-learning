#include "GradientTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_gradient.h"
#include <QRadioButton>

using namespace Gradient;

GradientTab::GradientTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    InitializePixelsWhite();

    Point p0(-200, -250, 0.3);
    Point p1(200, 50, 0.1);
    Point p2(20, 250, 1.0);

    DrawShadedTriangle(p0, p1, p2, Color(0, 255, 0));

    painter->render(pixels);
}