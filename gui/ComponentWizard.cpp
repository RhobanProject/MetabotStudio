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

ComponentWizard::ComponentWizard(Viewer *viewer_,
                                 Metabot::Robot *robot_,
                                 Metabot::AnchorPoint *anchor_,
                                 QWidget *parent) :
    viewer(viewer_),
    robot(robot_),
    anchor(anchor_),
    QDialog(parent),
    ui(new Ui::ComponentWizard),
    instance(NULL),
    previousInstance(NULL),
    previousAnchor(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Component wizard");

    ui->anchor->hide();
    ui->parameters->hide();
    ui->generate->hide();
    ui->ok->setEnabled(false);

    if (anchor == NULL) {
        if (robot->root != NULL) {
            previousInstance = instance = robot->root;
            setupInstance();
        }
    } else {
        if (anchor->anchor != NULL) {
            previousAnchor = anchor->anchor;
            instance = previousAnchor->instance;
            previousInstance = instance;
            setupInstance();
        }
    }

    fill();
}

ComponentWizard::~ComponentWizard()
{
    delete ui;
}

void ComponentWizard::fill()
{
    std::map<std::string, Metabot::Component*>::iterator it;
    for (it=robot->backend->components.begin();
         it!=robot->backend->components.end(); it++) {
        Metabot::Component *component = (*it).second;
        Metabot::ComponentInstance *instance = component->instanciate();
        instance->compile();

        if (instance->isCompatible(anchor)) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setData(ROLE_COMPONENT, QString::fromStdString((*it).first));
            ComponentItem *widget = new ComponentItem();
            widget->setInfo(component->prettyName, component->description);
            item->setSizeHint(widget->size());
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, widget);
        }
    }
}

void ComponentWizard::setupInstance()
{
    setAnchor(NULL);
    ui->welcome->hide();

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
    QRadioButton *first = NULL;
    for (it=instance->anchors.begin(); it!=instance->anchors.end(); it++) {
        Metabot::AnchorPoint *anchorPoint = *it;
        if (anchorPoint->isCompatible(anchor)) {
            index++;
            QString label = QString("%1").arg(index);
            QRadioButton *btn = new QRadioButton();
            if (first == NULL) {
                first = btn;
            }
            buttonToAnchor[btn] = *it;
            btn->setText(label);
            ui->anchor_items->addWidget(btn);
            QObject::connect(btn, SIGNAL(clicked()), this, SLOT(on_anchorPoint_clicked()));
        }
    }
    if (first != NULL) {
        first->setChecked(true);
        setAnchor(buttonToAnchor[first]);
    }
    if (buttonToAnchor.size() <= 1) {
        ui->anchor->hide();
    } else {
        ui->anchor->show();
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
    ui->parameters->show();
    }

    ui->generate->show();
    ui->ok->setEnabled(true);
    updateGeometry();
    update();
}

void ComponentWizard::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    if (items.size() == 1) {
        QListWidgetItem *item = items[0];
        QString data = item->data(ROLE_COMPONENT).toString();

        if (instance != NULL && instance != previousInstance) {
            delete instance;
        }

        // Create the instance
        instance = robot->backend->getComponent(data.toStdString())->instanciate();
        instance->compile();
        setupInstance();
    }
}

void ComponentWizard::setAnchor(Metabot::AnchorPoint *newAnchor)
{
    currentAnchor = newAnchor;
    if (anchor == NULL) {
        if (currentAnchor != NULL) {
            robot->root = currentAnchor->instance;
            viewer->updateRatio();
        } else {
            robot->root = NULL;
        }
    } else {
        anchor->attach(currentAnchor);
    }
}

void ComponentWizard::on_anchorPoint_clicked()
{
    setAnchor(buttonToAnchor[dynamic_cast<QWidget*>(sender())]);
}

void ComponentWizard::on_generate_clicked()
{
    Metabot::ComponentInstance *previous = instance;

    std::string name = instance->component->name;
    instance = robot->backend->getComponent(name)->instanciate();

    std::vector<ParameterWidget*>::iterator rit;
    for (rit=parameters.begin(); rit!=parameters.end(); rit++) {
        ParameterWidget *parameter = *rit;
        instance->values[parameter->name] = parameter->getValue();
    }
    instance->compile();

    if (previousInstance) {
        instance->merge(previousInstance, false);
    } else {
        instance->merge(previous, true);
    }

    if (previous != previousInstance) {
        delete previous;
    }
    setupInstance();
}

void ComponentWizard::cancel()
{
    if (anchor != NULL) {
        if (currentAnchor != previousAnchor) {
            anchor->detach();
            anchor->attach(previousAnchor);
        }
    } else {
        robot->root = previousInstance;
    }
    if (instance != previousInstance) {
        instance->detachAll();
        delete instance;
    }
    this->close();
}

void ComponentWizard::on_cancel_clicked()
{
    cancel();
    on_cancel();
}

void ComponentWizard::on_ok_clicked()
{
    if (previousInstance != NULL && instance != previousInstance) {
        // XXX: memory leak
        previousInstance->detachAll();
        delete previousInstance;
    }
    on_ok();
}
