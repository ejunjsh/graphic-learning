#include "mainwindow.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "painter.h"
#include "BasicRaysTab.h"
#include "DiffuseTab.h"
#include "SpecularTab.h"
#include "ShadowTab.h"
#include "ReflectionTab.h"
#include "CameraTab.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set the fixed size of the main window
    setFixedSize(700, 700);
    // Set the window title
    setWindowTitle("graphic-learning");
    // Create a central widget
    QWidget *centralWidget = new QWidget(this);

    // Create a layout and add it to the central widget
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QTabWidget *tabWidget = new QTabWidget(centralWidget);

    BasicRaysTab *basicRaysTab = new BasicRaysTab();
    tabWidget->addTab(basicRaysTab, "basic rays");

    DiffuseTab *diffuseTab = new DiffuseTab();
    tabWidget->addTab(diffuseTab, "diffuse");

    SpecularTab *specularTab = new SpecularTab();
    tabWidget->addTab(specularTab, "specular");

    ShadowTab *shadowTab = new ShadowTab();
    tabWidget->addTab(shadowTab, "shadow");

    ReflectionTab *reflectionTab = new ReflectionTab();
    tabWidget->addTab(reflectionTab, "reflection");

    CameraTab *cameraTab = new CameraTab();
    tabWidget->addTab(cameraTab, "camera");

    layout->addWidget(tabWidget);

    // Set the central widget for the main window
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{

}