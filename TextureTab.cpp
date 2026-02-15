#include "TextureTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>  
#include <QDebug>
#include <QPushButton>
#include "config/config_texture.h"

using namespace TextureDemo;

TextureTab::TextureTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    // Option widgets
    QHBoxLayout* perspectiveCorrectDepthLayout = new QHBoxLayout();
    QLabel *perspectiveCorrectDepthLabel = new QLabel("Perspective Correct Depth:", this);
    QRadioButton *linearOption = new QRadioButton("Linear(Z)", this);
    QRadioButton *inverseOption = new QRadioButton("Inverse(1/Z)", this);

    QButtonGroup *perspectiveCorrectDepthGroup = new QButtonGroup(this);
    perspectiveCorrectDepthGroup->addButton(linearOption, 0);
    perspectiveCorrectDepthGroup->addButton(inverseOption, 1);

    perspectiveCorrectDepthLayout->addWidget(perspectiveCorrectDepthLabel);
    perspectiveCorrectDepthLayout->addWidget(linearOption);
    perspectiveCorrectDepthLayout->addWidget(inverseOption);
    inverseOption->setChecked(true);

    connect(perspectiveCorrectDepthGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        if (btn == linearOption) {
            UsePerspectiveCorrectDepth = false;
            qDebug() << "UsePerspectiveCorrectDepth set to false (linear depth)";
        } else {
            UsePerspectiveCorrectDepth = true;
            qDebug() << "UsePerspectiveCorrectDepth set to true (inverse depth)";
        }
        RenderToPixels();
        painter->render(pixels);
    });

    mainLayout->addLayout(perspectiveCorrectDepthLayout);

    auto wood_texture = std::make_shared<Texture>(":/cgfs/textures-demo/crate-texture.jpg");

    // set texture for all triangles
    for (auto& tri : triangles) {    
        tri.setTexture(wood_texture);
    }

    RenderToPixels();

    painter->render(pixels);
}