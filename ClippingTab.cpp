#include "ClippingTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_clipping.h"

using namespace Clipping;

ClippingTab::ClippingTab(QWidget *parent)
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

    auto cube = Model(vertexes, triangles, Vertex(0,0,0), std::sqrt(3.0));
    auto instances = std::vector<Instance>{
            Instance(cube, Vertex{-1.5, 0, 7}, Identity4x4, 0.75),
            Instance(cube, Vertex{1.25, 2.5, 7.5}, MakeOYRotationMatrix(195))
    };

    auto camera = Camera(Vertex{-3, 1, 2}, MakeOYRotationMatrix(-30));

        // Clipping planes (near, left, right, top, bottom) with unit normals
    double s2 = std::sqrt(2.0) / 2.0;
    camera.clipping_planes = {
        Plane(Vertex(0, 0, 1), -1.0),        // Near
        Plane(Vertex(s2, 0, s2), 0.0),       // Left
        Plane(Vertex(-s2, 0, s2), 0.0),      // Right
        Plane(Vertex(0, -s2, s2), 0.0),      // Top
        Plane(Vertex(0, s2, s2), 0.0)        // Bottom
    };

    RenderScene(camera, instances);

    painter->render(pixels);
}