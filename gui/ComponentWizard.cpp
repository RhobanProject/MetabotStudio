#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"

ComponentWizard::ComponentWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentWizard)
{
    ui->setupUi(this);
    setWindowTitle("Component wizard");
}

ComponentWizard::~ComponentWizard()
{
    delete ui;
}
