#include "ShadowTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>  
#include <QDebug>
#include "painter.h"
#include "config/config_shadow.h"

using namespace Shadow;

ShadowTab::ShadowTab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    Painter *painter = new Painter(this);
    layout->addWidget(painter);
    setLayout(layout);

    QHBoxLayout *hLayout = new QHBoxLayout();
    QLabel *shadowLabel = new QLabel("Shadow ray start:", this);
    QRadioButton *zeroOption = new QRadioButton("Zero", this);
    QRadioButton *epsilonOption = new QRadioButton("Epsilon", this);

    hLayout->addWidget(shadowLabel);
    hLayout->addWidget(zeroOption);
    hLayout->addWidget(epsilonOption);

    QButtonGroup *shadowGroup = new QButtonGroup(this);
    shadowGroup->addButton(zeroOption, 0);
    shadowGroup->addButton(epsilonOption, 1);

    epsilonOption->setChecked(true);

    auto renderShadowScene = [=]() {
        Color (*pixels)[CANVAS_HEIGHT] = new Color[CANVAS_WIDTH][CANVAS_HEIGHT];

        Vector camera_position = CAMERA_POSITION;
        for (int x = -CANVAS_WIDTH / 2; x < CANVAS_WIDTH / 2; ++x) {
            for (int y = -CANVAS_HEIGHT / 2; y < CANVAS_HEIGHT / 2; ++y) {
                Vector direction = CanvasToViewport(x, y);
                Color color = TraceRay(camera_position, direction, 1.0, std::numeric_limits<double>::infinity());

                // Convert (x, y) from viewport to canvas coordinates
                int canvas_x = CANVAS_WIDTH / 2 + x;
                int canvas_y = CANVAS_HEIGHT / 2 - y - 1;

                if (canvas_x >= 0 && canvas_x < CANVAS_WIDTH && canvas_y >= 0 && canvas_y < CANVAS_HEIGHT) {
                    pixels[canvas_x][canvas_y] = color;
                }
            }
        }

        painter->render(pixels);
        delete[] pixels;
    };

    connect(shadowGroup, &QButtonGroup::buttonToggled,
            this, [=](QAbstractButton *btn, bool checked){
        if (!checked) return;
        if (btn == zeroOption) {
            EPSILON = 0.0;
            qDebug() << "EPSILON set to 0.0";
        } else {
            EPSILON = 0.001;
            qDebug() << "EPSILON set to 0.001";
        }
        renderShadowScene();
    });

    layout->addLayout(hLayout);

    renderShadowScene();
}