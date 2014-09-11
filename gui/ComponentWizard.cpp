#include <iostream>
#include "ComponentItem.h"
#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"
#include <metabot/ComponentInstance.h>
#include <metabot/ComponentParameter.h>
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
    currentAnchor = NULL;

    // Giving it to the viewer
    viewer->setInstance(instance);

    // Anchor points
    {
    std::map<QWidget*, Metabot::AnchorPoint*>::iterator rit;
    for (rit=buttonToAnchor.begin(); rit!=buttonToAnchor.end(); rit++) {
        QWidget *widget = rit->first;
        ui->anchor_items->removeWidget(widget);
        delete widget;
    }
    buttonToAnchor.clear();

    int index;
    std::vector<Metabot::AnchorPoint*>::iterator it;
    for (it=instance->anchors.begin(); it!=instance->anchors.end(); it++) {
        index++;
        QString label = QString("%1").arg(index);
        QRadioButton *btn = new QRadioButton();
        buttonToAnchor[btn] = *it;
        btn->setText(label);
        ui->anchor_items->addWidget(btn);
        QObject::connect(btn, SIGNAL(clicked()), this, SLOT(on_anchorPoint_clicked()));
    }
    }

    // Parameters
    {
    std::vector<ParameterWidget*>::iterator rit;
    for (rit=parameters.begin(); rit!=parameters.end(); rit++) {
        ui->parameters_items->removeWidget(*rit);
        delete *rit;
    }
    parameters.clear();

    std::map<std::string, Metabot::ComponentParameter*>::iterator pit;
    for (pit = instance->component->parameters.begin();
         pit != instance->component->parameters.end(); pit++) {
        std::string name = pit->first;
        ParameterWidget *parameterWidget = new ParameterWidget(instance, name);
        parameters.push_back(parameterWidget);
        ui->parameters_items->addWidget(parameterWidget);
    }
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
    currentAnchor = anchor;
    viewer->matrix = anchor->matrix.invert();
}

void ComponentWizard::on_anchorPoint_clicked()
{
    setAnchor(buttonToAnchor[dynamic_cast<QWidget*>(sender())]);
}

void ComponentWizard::on_generate_clicked()
{
    std::vector<ParameterWidget*>::iterator rit;
    for (rit=parameters.begin(); rit!=parameters.end(); rit++) {
        ParameterWidget *parameter = *rit;
        instance->values[parameter->name] = parameter->getValue();
        instance->compile();
    }
}
