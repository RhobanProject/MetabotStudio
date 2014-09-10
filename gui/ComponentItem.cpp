#include "ComponentItem.h"
#include "ui_ComponentItem.h"

ComponentItem::ComponentItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComponentItem)
{
    ui->setupUi(this);
}

ComponentItem::~ComponentItem()
{
    delete ui;
}

void ComponentItem::setInfo(std::string name, std::string description)
{
    ui->name->setText(QString("<b>%1</b>").arg(QString::fromStdString(name)));
    ui->description->setText(QString("<i>%1</i>").arg(QString::fromStdString(description)));
}
