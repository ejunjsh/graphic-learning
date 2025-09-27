#include "SceneTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_scene.h"
#include <QRadioButton>

using namespace Scene;

SceneTab::SceneTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    InitializePixelsWhite();

    Vertex vA(-2, -0.5, 5);
    Vertex vB(-2, 0.5, 5);
    Vertex vC(-1, 0.5, 5);
    Vertex vD(-1, -0.5, 5);

    Vertex vAb(-2, -0.5, 6);
    Vertex vBb(-2, 0.5, 6);
    Vertex vCb(-1, 0.5, 6);
    Vertex vDb(-1, -0.5, 6);

    Color RED(255, 0, 0);
    Color GREEN(0, 255, 0);
    Color BLUE(0, 0, 255);

    // Draw front face
    DrawLine(ProjectVertex(vA), ProjectVertex(vB), BLUE);
    DrawLine(ProjectVertex(vB), ProjectVertex(vC), BLUE);
    DrawLine(ProjectVertex(vC), ProjectVertex(vD), BLUE);
    DrawLine(ProjectVertex(vD), ProjectVertex(vA), BLUE);

    // Draw back face
    DrawLine(ProjectVertex(vAb), ProjectVertex(vBb), RED);
    DrawLine(ProjectVertex(vBb), ProjectVertex(vCb), RED);
    DrawLine(ProjectVertex(vCb), ProjectVertex(vDb), RED);
    DrawLine(ProjectVertex(vDb), ProjectVertex(vAb), RED);

    // Draw connecting edges
    DrawLine(ProjectVertex(vA), ProjectVertex(vAb), GREEN);
    DrawLine(ProjectVertex(vB), ProjectVertex(vBb), GREEN);
    DrawLine(ProjectVertex(vC), ProjectVertex(vCb), GREEN);
    DrawLine(ProjectVertex(vD), ProjectVertex(vDb), GREEN);

    painter->render(pixels);
}