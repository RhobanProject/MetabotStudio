#include <iostream>
#include "ComponentItem.h"
#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"
#include <metabot/ComponentInstance.h>
#include <metabot/Backend.h>

#define ROLE_COMPONENT  1002

ComponentWizard::ComponentWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentWizard),
    instance(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Component wizard");

    viewer = new Viewer;
    ui->zone->addWidget(viewer);
}

ComponentWizard::~ComponentWizard()
{
    delete viewer;
    delete ui;
}

void ComponentWizard::setBackend(Metabot::Backend *backend_)
{
    backend = backend_;
    fill();
}

void ComponentWizard::fill()
{
    std::map<std::string, Metabot::Component*>::iterator it;
    for (it=backend->components.begin(); it!=backend->components.end(); it++) {
        Metabot::Component *component = (*it).second;
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(ROLE_COMPONENT, QString::fromStdString((*it).first));
        ComponentItem *widget = new ComponentItem();
        widget->setInfo(component->prettyName, component->description);
        item->setSizeHint(widget->size());
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void ComponentWizard::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    if (items.size() == 1) {
        QListWidgetItem *item = items[0];
        QString data = item->data(ROLE_COMPONENT).toString();
        instance = backend->getComponent(data.toStdString())->instanciate();
        instance->compile();
        viewer->setInstance(instance);
    }
}
