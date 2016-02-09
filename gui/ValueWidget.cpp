#include <QHBoxLayout>
#include "ValueWidget.h"

ValueWidget::ValueWidget(QWidget *parent) :
    QWidget(parent),
    key(this), value(this), min(this), max(this)
{
    this->setLayout(new QHBoxLayout());
    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);
    this->layout()->addWidget(&key);
    this->layout()->addWidget(&value);
    this->layout()->addWidget(&min);
    this->layout()->addWidget(&max);
}

ValueWidget::~ValueWidget()
{

}

void ValueWidget::set(QString key_, Metabot::Parameter param)
{
    key.setText(key_);
    value.setText(QString::fromStdString(param.value));
    min.setText(QString::fromStdString(param.min));
    max.setText(QString::fromStdString(param.max));
}

QString ValueWidget::getKey()
{
    return key.text();
}

Metabot::Parameter ValueWidget::getValue()
{
    Metabot::Parameter p;

    p.value = value.text().toStdString();
    p.min = min.text().toStdString();
    p.max = max.text().toStdString();

    return p;
}
