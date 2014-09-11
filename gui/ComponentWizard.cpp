#include <iostream>
#include "ComponentItem.h"
#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"
#include <metabot/ComponentInstance.h>
#include <metabot/AnchorPoint.h>
#include <metabot/Backend.h>
#include <QRadioButton>

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

void ComponentWizard::setupInstance()
{
    // Giving it to the viewer
    viewer->setInstance(instance);

    // Anchor points
    std::vector<QRadioButton*>::iterator rit;
    for (rit=anchorButtons.begin(); rit!=anchorButtons.end(); rit++) {
        ui->anchor_items->removeWidget(*rit);
        delete *rit;
    }
    anchorButtons.clear();
    buttonToAnchor.clear();

    int index;
    std::vector<Metabot::AnchorPoint*>::iterator it;
    for (it=instance->anchors.begin(); it!=instance->anchors.end(); it++) {
        index++;
        QString label = QString("%1").arg(index);
        QRadioButton *btn = new QRadioButton();
        buttonToAnchor[btn] = *it;
        anchorButtons.push_back(btn);
        btn->setText(label);
        ui->anchor_items->addWidget(btn);
        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(on_anchorPoint_clicked()));
    }
}

void ComponentWizard::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    if (items.size() == 1) {
        QListWidgetItem *item = items[0];
        QString data = item->data(ROLE_COMPONENT).toString();

        if (instance != NULL) {
            delete instance;
        }

        // Create the instance
        instance = backend->getComponent(data.toStdString())->instanciate();
        instance->compile();
        setupInstance();
    }
}

void ComponentWizard::setAnchor(Metabot::AnchorPoint *anchor)
{
    viewer->matrix = anchor->matrix.invert();
}

void ComponentWizard::on_anchorPoint_clicked()
{
    setAnchor(buttonToAnchor[dynamic_cast<QRadioButton*>(sender())]);
}
