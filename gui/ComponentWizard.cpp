#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"

ComponentWizard::ComponentWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentWizard)
{
    ui->setupUi(this);
    setWindowTitle("Component wizard");

    QListWidgetItem *item = new QListWidgetItem();
    ui->listWidget->addItem(item);
}

ComponentWizard::~ComponentWizard()
{
    delete ui;
}

void ComponentWizard::setBackend(Metabot::Backend *backend_)
{
    backend = backend_;
}
