#include <iostream>
#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Metabot");

    // Loading metabot backend
    backend = new Metabot::Backend("xl-320");
    backend->load();

    // Component wizard dialog
    wizard = new ComponentWizard();
    wizard->setBackend(backend);
    wizard->show();
    // XXX: Hack to prevent premature GL painting
    wizard->viewer->canPaint = true;
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}
