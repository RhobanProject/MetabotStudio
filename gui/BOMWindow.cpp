#include "BOMWindow.h"
#include "ui_BOMWindow.h"

BOMWindow::BOMWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BOMWindow)
{
    ui->setupUi(this);
}

BOMWindow::~BOMWindow()
{
    delete ui;
}

void BOMWindow::setBom(QString text)
{
    ui->bom->setText(text);
    ui->bom->setOpenExternalLinks(true);
}

void BOMWindow::on_ok_clicked()
{
    this->close();
}
