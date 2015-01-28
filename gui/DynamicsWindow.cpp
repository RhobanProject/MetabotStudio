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
