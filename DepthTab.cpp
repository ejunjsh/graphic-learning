#include "DepthTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_depth.h"
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>  
#include <QDebug>
#include <QPushButton>

using namespace Depth;

DepthTab::DepthTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    // Option widgets
    QHBoxLayout *optionsLayout1 = new QHBoxLayout();
    QLabel *label = new QLabel("Depth buffering & Backface culling:", this);
    QRadioButton *option1 = new QRadioButton("Disabled", this);
    QRadioButton *option2 = new QRadioButton("Enabled", this);
    
    QButtonGroup *depthGroup = new QButtonGroup(this);
    depthGroup->addButton(option1, 0);
    depthGroup->addButton(option2, 1);

    // set initial state from depthChecksEnabled
    option2->setChecked(depthChecksEnabled);
    option1->setChecked(!depthChecksEnabled);

    optionsLayout1->addWidget(label);
    optionsLayout1->addWidget(option1);
    optionsLayout1->addWidget(option2);

    bool* depthChecksEnabledPtr = &depthChecksEnabled;
    connect(depthGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        // option2 = Enabled
        *depthChecksEnabledPtr = (btn == option2);
        if(btn == option1) {
            qDebug() << "Depth buffering & Backface culling disabled";
        } else {
            qDebug() << "Depth buffering & Backface culling enabled";
        }
        RenderToPixels(false);
        painter->render(pixels);
    });

    QHBoxLayout* optionsLayout3 = new QHBoxLayout();

    QLabel *label2 = new QLabel("Triangle outlines", this);
    QRadioButton *option5 = new QRadioButton("Disabled", this);
    QRadioButton *option6 = new QRadioButton("Enabled", this);

    QButtonGroup *outlineGroup = new QButtonGroup(this);
    outlineGroup->addButton(option5, 0);
    outlineGroup->addButton(option6, 1);

    // set initial state from drawOutlinesEnabled
    option6->setChecked(drawOutlinesEnabled);
    option5->setChecked(!drawOutlinesEnabled);

    optionsLayout3->addWidget(label2);
    optionsLayout3->addWidget(option5);
    optionsLayout3->addWidget(option6);

    bool* drawOutlinesEnabledPtr = &drawOutlinesEnabled;
    connect(outlineGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        // option6 = Enabled
        *drawOutlinesEnabledPtr = (btn == option6);
        if(btn == option5) {
            qDebug() << "Triangle outlines disabled";
        } else {
            qDebug() << "Triangle outlines enabled";
        }
        RenderToPixels(false);
        painter->render(pixels);
    });

    
    QHBoxLayout* optionsLayout = new QHBoxLayout();
    optionsLayout->addLayout(optionsLayout1);
    optionsLayout->addLayout(optionsLayout3);

    QPushButton *shuffleBtn = new QPushButton("Shuffle triangles", this);

    connect(shuffleBtn, &QPushButton::clicked, this, [=](){
        qDebug() << "Shuffle triangles button clicked";
        RenderToPixels(true);
        painter->render(pixels);
    });

    

    mainLayout->addLayout(optionsLayout);

    mainLayout->addWidget(shuffleBtn);

    RenderToPixels(false);

    painter->render(pixels);
}


  