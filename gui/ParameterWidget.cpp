#include <iostream>
#include "ParameterWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <metabot/Component.h>
#include <metabot/Parameter.h>
#include <metabot/scad/Module.h>

ParameterWidget::ParameterWidget(Metabot::Component *instance_,
                                 std::string name_, QWidget *parent) :
    instance(instance_),
    name(name_),
    QWidget(parent),
    label(NULL), line(NULL), checkbox(NULL)
{
    auto param = instance->module->getParameter(name);
    std::string value = instance->get(param.name);

    label = new QLabel;
    label->setText(QString::fromStdString(param.description));

    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(label);

    if (param.isBool()) {
        checkbox = new QCheckBox;
        checkbox->setChecked(value=="true");
        this->layout()->addWidget(checkbox);
    } else {
        line = new QLineEdit;
        line->setText(QString::fromStdString(value));
        this->layout()->addWidget(line);
    }

    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);
    this->layout()->update();
    this->adjustSize();
    this->update();
    this->show();
}

ParameterWidget::~ParameterWidget()
{
    if (label) delete label;
    if (line) delete line;
    if (checkbox) delete checkbox;
}

std::string ParameterWidget::getValue()
{
    if (checkbox != NULL) {
        return checkbox->isChecked() ? "true" : "false";
    }
    if (line != NULL) {
        return line->text().toStdString();
    }

    return "";
}
