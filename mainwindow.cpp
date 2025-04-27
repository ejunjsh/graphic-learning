#include "mainwindow.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set the window resolution to 1080p
    resize(1280, 720);
    // Set the window title
    setWindowTitle("graphic-learning");
    // Create a central widget
    QWidget *centralWidget = new QWidget(this);

    // Create a layout and add it to the central widget
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QTabWidget *tabWidget = new QTabWidget(centralWidget);

    QWidget *page1 = new QWidget();
    QLabel *label1 = new QLabel("Content of Tab 1", page1);
    QVBoxLayout *page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(label1);
    page1->setLayout(page1Layout);
    tabWidget->addTab(page1, "Tab 1");

    QWidget *page2 = new QWidget();
    QLabel *label2 = new QLabel("Content of Tab 2", page2);
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(label2);
    page2->setLayout(page2Layout);
    tabWidget->addTab(page2, "Tab 2");

    layout->addWidget(tabWidget);

    // Set the central widget for the main window
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}