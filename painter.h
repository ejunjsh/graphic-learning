#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>

class Painter : public QWidget {
    Q_OBJECT

public:
    Painter(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // PAINTER_H