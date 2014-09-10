#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Metabot");
    wizard = new ComponentWizard();
    backend = new Metabot::Backend("xl-320");
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}
