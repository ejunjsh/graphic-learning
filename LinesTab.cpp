#include "LinesTab.h"
#include <QVBoxLayout>
#include "painter.h"
#include "config/config_lines.h"
#include <QRadioButton>

using namespace Lines;

LinesTab::LinesTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    Painter *painter = new Painter(this);
    mainLayout->addWidget(painter);
    setLayout(mainLayout);

    // Option widgets
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QRadioButton *option1 = new QRadioButton("Draw Broken Line", this);
    QRadioButton *option2 = new QRadioButton("Draw Line", this);
 
    optionsLayout->addWidget(option1);
    optionsLayout->addWidget(option2);

    mainLayout->addLayout(optionsLayout);

    // Callback when option changed
    QObject::connect(option1, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) {
            InitializePixelsWhite();
            DrawLineBroken(-200, -100, 240, 120, Color(0, 0, 0));
            DrawLineBroken(-50, -200, 60, 240, Color(0, 0, 0));
            painter->render(pixels);
        }
    });

    QObject::connect(option2, &QRadioButton::toggled, this, [=](bool checked){
        if (checked) {
            InitializePixelsWhite();
            DrawLine(Point(-200, -100), Point(240, 120), Color(0, 0, 0));
            DrawLine(Point(-50, -200), Point(60, 240), Color(0, 0, 0));
            painter->render(pixels);
        }
    });

    option1->setChecked(true); // Set the first option checked
}