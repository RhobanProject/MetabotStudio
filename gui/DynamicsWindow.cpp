#include "DynamicsWindow.h"
#include "ui_DynamicsWindow.h"

DynamicsWindow::DynamicsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DynamicsWindow)
{
    ui->setupUi(this);
}

DynamicsWindow::~DynamicsWindow()
{
    delete ui;
}

void DynamicsWindow::setDynamics(Metabot::Dynamics dynamics)
{
    ui->dynamics->setText(QString::fromStdString(dynamics.toString()));
}

void DynamicsWindow::on_pushButton_clicked()
{
    this->close();
}
