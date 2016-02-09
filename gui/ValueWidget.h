#ifndef VALUEWIDGET_H
#define VALUEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <Parameters.h>

class ValueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ValueWidget(QWidget *parent = 0);
    virtual ~ValueWidget();

    void set(QString key, Metabot::Parameter parameter);
    QString getKey();
    Metabot::Parameter getValue();

protected:
    QLineEdit key, value, min, max;

signals:

public slots:

};

#endif // VALUEWIDGET_H
