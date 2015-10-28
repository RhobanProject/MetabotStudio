#ifndef VALUEWIDGET_H
#define VALUEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class ValueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ValueWidget(QWidget *parent = 0);
    virtual ~ValueWidget();

    void set(QString key, QString value);
    QString getKey();
    QString getValue();

protected:
    QLineEdit key, value;

signals:

public slots:

};

#endif // VALUEWIDGET_H
