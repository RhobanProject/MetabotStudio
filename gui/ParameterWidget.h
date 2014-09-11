#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QWidget>
#include <metabot/ComponentInstance.h>

class ParameterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParameterWidget(Metabot::ComponentInstance *instance, std::string name, QWidget *parent = 0);
    virtual ~ParameterWidget();
    std::string getValue();

    Metabot::ComponentInstance *instance;
    std::string name;

protected:
    QLabel *label;
    QLineEdit *line;
    QCheckBox *checkbox;
    
signals:
    
public slots:
    
};

#endif // PARAMETERWIDGET_H
