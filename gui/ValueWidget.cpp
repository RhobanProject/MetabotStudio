#include <QHBoxLayout>
#include "ValueWidget.h"

ValueWidget::ValueWidget(QWidget *parent) :
    QWidget(parent),
    key(this), value(this)
{
    this->setLayout(new QHBoxLayout());
    this->layout()->setSpacing(0);
    this->layout()->setMargin(0);
    this->layout()->addWidget(&key);
    this->layout()->addWidget(&value);
}

ValueWidget::~ValueWidget()
{

}

void ValueWidget::set(QString key_, QString value_)
{
    key.setText(key_);
    value.setText(value_);
}

QString ValueWidget::getKey()
{
    return key.text();
}

QString ValueWidget::getValue()
{
    return value.text();
}
