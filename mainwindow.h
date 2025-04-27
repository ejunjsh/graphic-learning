#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// #include <QWidget>
// #include <QTabWidget>
// #include <QVBoxLayout>
// #include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

// private:
//      QVBoxLayout *layout;
//      QTabWidget *tabWidget;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H