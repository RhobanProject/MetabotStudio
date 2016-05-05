#include <iostream>
#include "ComponentItem.h"
#include "ComponentWizard.h"
#include "ui_ComponentWizard.h"
#include <metabot/Component.h>
#include <metabot/Parameter.h>
#include <metabot/AnchorPoint.h>
#include <metabot/Backend.h>
#include <metabot/util/util.h>
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
    ui->nameBox->hide();
    ui->anchor->hide();
    ui->orientation->hide();
    ui->range->hide();
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
            instance = previousAnchor->component;
            setupInstance();
            setAnchor(previousAnchor);
        }
    }

    fill();
}

ComponentWizard::~ComponentWizard()
{
    for (auto parameter : parameters) {
        delete parameter;
    }

    delete ui;
}

void ComponentWizard::fill()
{
    // Populating components choice, based on anchor compatibility
    auto modules = robot->backend->getModules();

    for (auto module : modules) {
        if (module->getType() == "component") {
            auto *instance = robot->backend->instanciate(module->getName());
            instance->compile();

            // If we're working on the robot root, any instance is OK
            if (anchor == NULL || instance->isCompatible(anchor)) {
                QListWidgetItem *item = new QListWidgetItem();
                item->setData(ROLE_COMPONENT, QString::fromStdString(module->getName()));
                ComponentItem *widget = new ComponentItem();
                widget->setInfo(module->getPrettyName(), module->getDescription());
                item->setSizeHint(widget->size());
                ui->listWidget->addItem(item);
                ui->listWidget->setItemWidget(item, widget);
            }

            delete instance;
        }
    }
}

void ComponentWizard::setupInstance()
{
    ui->scrollArea->show();
    ui->welcome->hide();

    // Instance name
    ui->nameBox->show();
    ui->name->setText(QString::fromStdString(instance->name));

    if (anchor == NULL) {
        // We're working on robot root, anchor doesn't really matter
        bool setRatio = (robot->root == NULL);
        robot->root = instance;

        if (setRatio) {
            viewer->updateRatio();
        }
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
        ui->orientationX->setText(QString("%1").arg(RAD2DEG(anchor->orientationX)));
        ui->orientationY->setText(QString("%1").arg(RAD2DEG(anchor->orientationY)));
        ui->orientationZ->setText(QString("%1").arg(RAD2DEG(anchor->orientationZ)));

        // Range
        ui->range->show();
        ui->min->setText(QString("%1").arg(RAD2DEG(anchor->minimum)));
        ui->max->setText(QString("%1").arg(RAD2DEG(anchor->maximum)));
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
        for (auto entry : instance->module->getParameters()) {
            auto parameter = entry.second;
            ParameterWidget *parameterWidget = new ParameterWidget(instance, parameter.name);
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
        instance = robot->backend->instanciate(data.toStdString());
        instance->compile(robot);

        if (oldInstance != NULL) {
            instance->moveAnchors(oldInstance->anchors);
            delete oldInstance;
        }

        setupInstance();
    }

    viewer->redraw();
}

void ComponentWizard::setAnchor(Metabot::AnchorPoint *newAnchor)
{
    currentAnchor = newAnchor;

    if (anchor == NULL) {
        if (currentAnchor != NULL) {
            robot->root = currentAnchor->component;
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

    viewer->redraw();
}

void ComponentWizard::on_anchorPoint_clicked()
{
    setAnchor(buttonToAnchor[dynamic_cast<QWidget*>(sender())]);
    viewer->redraw();
}

void ComponentWizard::on_generate_clicked()
{
    auto previous = instance;

    std::string name = instance->module->getName();
    instance = robot->backend->instanciate(name);

    for (auto parameter : parameters) {
        instance->parameters.set(parameter->name, parameter->getValue());
    }

    instance->compile(robot);
    instance->moveAnchors(previous->anchors);

    for (auto anchor : instance->anchors) {
        if (anchor->above == false) {
            previousAnchor = anchor;
        }
    }

    previous->detachAll();
    delete previous;

    setupInstance();
    viewer->redraw();
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

void ComponentWizard::on_orientationX_editingFinished()
{
    anchor->orientationX = DEG2RAD(ui->orientationX->text().toFloat());
    anchor->computeMatrixes();
    viewer->redraw();
}

void ComponentWizard::on_orientationY_editingFinished()
{
    anchor->orientationY = DEG2RAD(ui->orientationY->text().toFloat());
    anchor->computeMatrixes();
    viewer->redraw();
}

void ComponentWizard::on_orientationZ_editingFinished()
{
    anchor->orientationZ = DEG2RAD(ui->orientationZ->text().toFloat());
    anchor->computeMatrixes();
    viewer->redraw();
}

void ComponentWizard::on_min_editingFinished()
{
    anchor->minimum = DEG2RAD(ui->min->text().toFloat());
}

void ComponentWizard::on_max_editingFinished()
{
    anchor->maximum = DEG2RAD(ui->max->text().toFloat());
}

void ComponentWizard::on_name_editingFinished()
{
    if (instance) {
        instance->name = ui->name->text().toStdString();
    }
}
