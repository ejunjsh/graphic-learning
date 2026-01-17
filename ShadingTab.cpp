#include "ShadingTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_shading.h"
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>  
#include <QDebug>
#include <QPushButton>

using namespace Shading;

ShadingTab::ShadingTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    // Option widgets
    QHBoxLayout *lightingLayout = new QHBoxLayout();
    QLabel *lightingLabel = new QLabel("Lighting model:", this);
    QRadioButton *diffuseOption = new QRadioButton("Diffuse only", this);
    QRadioButton *specularOption = new QRadioButton("Specular only", this);
    QRadioButton *diffuseSpecularOption = new QRadioButton("Diffuse + Specular", this);

    QButtonGroup *lightingGroup = new QButtonGroup(this);
    lightingGroup->addButton(diffuseOption, 0);
    lightingGroup->addButton(specularOption, 1);
    lightingGroup->addButton(diffuseSpecularOption, 2);

    diffuseSpecularOption->setChecked(true);

    lightingLayout->addWidget(lightingLabel);
    lightingLayout->addWidget(diffuseOption);
    lightingLayout->addWidget(specularOption);
    lightingLayout->addWidget(diffuseSpecularOption);

    connect(lightingGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        if (btn == diffuseOption) {
            LightingModel = LM_DIFFUSE;
            qDebug() << "LightingModel set to LM_DIFFUSE";
        } else if (btn == specularOption) {
            LightingModel = LM_SPECULAR;
            qDebug() << "LightingModel set to LM_SPECULAR";
        } else {
            LightingModel = LM_DIFFUSE | LM_SPECULAR;
            qDebug() << "LightingModel set to LM_DIFFUSE | LM_SPECULAR";
        }
        RenderToPixels();
        painter->render(pixels);
    });

    QLabel *shadingLabel = new QLabel("Shading model:", this);
    QRadioButton *flatOption = new QRadioButton("Flat", this);
    QRadioButton *gouraudOption = new QRadioButton("Gouraud", this);
    QRadioButton *phongOption = new QRadioButton("Phong", this);

    QButtonGroup *shadingGroup = new QButtonGroup(this);
    shadingGroup->addButton(flatOption, 0);
    shadingGroup->addButton(gouraudOption, 1);
    shadingGroup->addButton(phongOption, 2);

    phongOption->setChecked(true);

    lightingLayout->addWidget(shadingLabel);
    lightingLayout->addWidget(flatOption);
    lightingLayout->addWidget(gouraudOption);
    lightingLayout->addWidget(phongOption);

    connect(shadingGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        if (btn == flatOption) {
            ShadingModel = SM_FLAT;
            qDebug() << "ShadingModel set to SM_FLAT";
        } else if (btn == gouraudOption) {
            ShadingModel = SM_GOURAUD;
            qDebug() << "ShadingModel set to SM_GOURAUD";
        } else {
            ShadingModel = SM_PHONG;
            qDebug() << "ShadingModel set to SM_PHONG";
        }
        RenderToPixels();
        painter->render(pixels);
    });

    QHBoxLayout* normalsLayout = new QHBoxLayout();
    QLabel *normalsLabel = new QLabel("Normals:", this);
    QRadioButton *computedNormalsOption = new QRadioButton("Computed triangle normals", this);
    QRadioButton *vertexNormalsOption = new QRadioButton("Vertex normals from model", this);

    QButtonGroup *normalsGroup = new QButtonGroup(this);
    normalsGroup->addButton(computedNormalsOption, 0);
    normalsGroup->addButton(vertexNormalsOption, 1);

    normalsLayout->addWidget(normalsLabel);
    normalsLayout->addWidget(computedNormalsOption);
    normalsLayout->addWidget(vertexNormalsOption);
    vertexNormalsOption->setChecked(true);

    connect(normalsGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        if (btn == computedNormalsOption) {
            UseVertexNormals = false;
            qDebug() << "UseVertexNormals set to false (computed triangle normals)";
        } else {
            UseVertexNormals = true;
            qDebug() << "UseVertexNormals set to true (vertex normals from model)";
        }
        RenderToPixels();
        painter->render(pixels);
    });

    mainLayout->addLayout(lightingLayout);

    mainLayout->addLayout(normalsLayout);

    RenderToPixels();

    painter->render(pixels);
}


  