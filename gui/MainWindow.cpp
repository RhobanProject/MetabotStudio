#include <iostream>
#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    robot(NULL), viewer(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Metabot");

    // Loading metabot backend
    backend = new Metabot::Backend("xl-320");
    backend->load();

    // Creating viewer
    viewer = new Viewer();
    ui->viewer->addWidget(viewer);

    // Creating robot
    robot = new Metabot::Robot(backend);
    viewer->setRobot(robot);

    Metabot::ComponentInstance *body = backend->getComponent("body")->instanciate();
    body->compile();
    robot->root = body;
    viewer->updateRatio();

    // Component wizard dialog
    wizard = new ComponentWizard(viewer, robot, body->anchors[1]);
    wizard->show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}
