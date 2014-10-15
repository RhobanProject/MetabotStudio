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
    previousAnchor(NULL)
{
    isOk = false;
    ui->setupUi(this);
    setWindowTitle("Component wizard");

    // Showing welcome menu only
    ui->scrollArea->hide();
    ui->anchor->hide();
    ui->orientation->hide();
    ui->parameters->hide();
    ui->generate->hide();
    ui->ok->setEnabled(false);

    if (anchor == NULL) {
        // We're working on root of the robot
        if (robot->root != NULL) {
            instance = robot->root;
            setupInstance();
        }
    } else {
        // We're working on a specific anchor
        if (anchor->anchor != NULL) {
            previousAnchor = anchor->anchor;
            instance = previousAnchor->instance;
            setupInstance();
            setAnchor(previousAnchor);
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
    // Populating components choice, based on anchor compatibility
    std::map<std::string, Metabot::Component*>::iterator it;
    for (it=robot->backend->components.begin();
         it!=robot->backend->components.end(); it++) {
        Metabot::Component *component = (*it).second;
        Metabot::ComponentInstance *instance = component->instanciate();
        instance->compile();

        // If we're working on the robot root, any instance is OK
        if (anchor == NULL || instance->isCompatible(anchor)) {
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
    ui->scrollArea->show();
    ui->welcome->hide();

    if (anchor == NULL) {
        // We're working on robot root, anchor doesn't really matter
        robot->root = instance;
        viewer->updateRatio();
    } else  {
        // Displaying anchor points
        for (auto entry : buttonToAnchor) {
            ui->anchor_items->removeWidget(entry.first);
            delete entry.first;
        }
        buttonToAnchor.clear();

        QRadioButton *first = NULL;
        bool firstBusy = false;
        QRadioButton *previous = NULL;

        for (auto anchorPoint : instance->anchors) {
            if (anchor != NULL && anchorPoint->isCompatible(anchor)) {
                QString label = QString("%1").arg(anchorPoint->id);
                QRadioButton *btn = new QRadioButton();
                if (first == NULL || firstBusy) {
                    first = btn;
                    firstBusy = anchorPoint->anchor != NULL;
                }
                if (anchorPoint == previousAnchor) {
                    previous = btn;
                }
                buttonToAnchor[btn] = anchorPoint;
                btn->setText(label);
                ui->anchor_items->addWidget(btn);
                QObject::connect(btn, SIGNAL(clicked()), this, SLOT(on_anchorPoint_clicked()));
            }
        }
        if (previous) {
            // We're working on instance that already had an anchor point,
            previous->setChecked(true);
        } else {
            // We're working on a fresh instance, let's take the first anchor
            // point as reference
            if (first != NULL) {
                first->setChecked(true);
                setAnchor(buttonToAnchor[first]);
            }
        }

        // If there is only one anchor, we can hide the menu for selecting it
        if (buttonToAnchor.size() <= 1) {
            ui->anchor->hide();
        } else {
            ui->anchor->show();
        }

        // Orientation
        ui->orientation->show();
        if (anchor->orientation == 0) {
            ui->orientation_normal->setChecked(true);
            ui->orientation_back->setChecked(false);
        } else {
            ui->orientation_normal->setChecked(false);
            ui->orientation_back->setChecked(true);
        }
    }
    ui->anchor->update();
    ui->anchor->adjustSize();

    // Parameters
    {
        for (auto parameter : parameters) {
            ui->parameters_items->removeWidget(parameter);
            delete parameter;
        }
        parameters.clear();

        // Adding parameter widgets
        for (auto parameter : instance->component->parameters) {
            std::string name = parameter.first;
            ParameterWidget *parameterWidget = new ParameterWidget(instance, name);
            parameters.push_back(parameterWidget);
            ui->parameters_items->addWidget(parameterWidget);
        }
        ui->parameters->show();
    }

    // Enabling buttons and adjusting size
    ui->verticalLayout->update();

    ui->ok->setEnabled(true);
    ui->generate->show();
}

void ComponentWizard::on_listWidget_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();

    if (items.size() == 1) {
        auto oldInstance = instance;
        QListWidgetItem *item = items[0];
        QString data = item->data(ROLE_COMPONENT).toString();

        if (anchor != NULL) {
            anchor->detach(false);
        }

        // Create the instance
        instance = robot->backend->getComponent(data.toStdString())->instanciate();
        instance->compile();

        if (oldInstance != NULL) {
            instance->moveAnchors(oldInstance);
            delete oldInstance;
        }

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
        Metabot::AnchorPoint *orphan = NULL;

        orphan = newAnchor->anchor;
        if (orphan != NULL) {
            if (orphan == anchor) {
                orphan = NULL;
            } else {
                orphan->detach(false);
            }
        }

        if (anchor->anchor != NULL) {
            anchor->anchor->detach(false);
            anchor->detach(false);
        }
        anchor->attach(currentAnchor);

        if (orphan != NULL) {
            auto candidate = instance->findCompatible(orphan);
            if (candidate) {
                candidate->attach(orphan);
            } else {
                delete orphan;
            }
        }
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
    instance->moveAnchors(previous);

    previous->detachAll();
    delete previous;

    setupInstance();
}

void ComponentWizard::closeEvent(QCloseEvent *)
{
    if (!isOk) {
        on_cancel();
    }
}

void ComponentWizard::on_cancel_clicked()
{
    close();
}

void ComponentWizard::on_ok_clicked()
{
    isOk = true;
    on_ok();
}

void ComponentWizard::on_orientation_back_clicked()
{
    anchor->orientation = M_PI;
}

void ComponentWizard::on_orientation_normal_clicked()
{
    anchor->orientation = 0;
}
