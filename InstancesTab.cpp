#include "InstancesTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_instances.h"
#include <QRadioButton>

using namespace Instances;

InstancesTab::InstancesTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    InitializePixelsWhite();

    auto vertexes = std::vector<Vertex>{
            Vertex{1, 1, 1}, Vertex{-1, 1, 1}, Vertex{-1, -1, 1}, Vertex{1, -1, 1},
            Vertex{1, 1, -1}, Vertex{-1, 1, -1}, Vertex{-1, -1, -1}, Vertex{1, -1, -1}
    };

    auto RED = Color(255, 0, 0);
    auto GREEN = Color(0, 255, 0);
    auto BLUE = Color(0, 0, 255);
    auto YELLOW = Color(255, 255, 0);
    auto PURPLE = Color(255, 0, 255);
    auto CYAN = Color(0, 255, 255);

    auto triangles = std::vector<Triangle>{
            // front face
            Triangle{0, 1, 2, RED}, Triangle{0, 2, 3, RED},
            // back face
            Triangle{4, 0, 3, GREEN}, Triangle{4, 3, 7, GREEN},
            // left face
            Triangle{5, 4, 7, BLUE}, Triangle{5, 7, 6, BLUE},
            // right face
            Triangle{1, 5, 6, YELLOW}, Triangle{1, 6, 2, YELLOW},
            // top face
            Triangle{4, 5, 1, PURPLE}, Triangle{4, 1, 0, PURPLE},
            // bottom face
            Triangle{2, 6, 7, CYAN}, Triangle{2, 7, 3, CYAN}
    };

    for(auto &v : vertexes) {
        v.x -= 1.5; 
        v.z += 7; 
    }

    RenderObject(vertexes, triangles);

    painter->render(pixels);
}