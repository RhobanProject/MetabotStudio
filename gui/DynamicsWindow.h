#ifndef DYNAMICSWINDOW_H
#define DYNAMICSWINDOW_H

#include <QDialog>
#include <metabot/Robot.h>

namespace Ui {
class DynamicsWindow;
}

class DynamicsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DynamicsWindow(QWidget *parent = 0);
    ~DynamicsWindow();

    void setDynamics(Metabot::Dynamics dynamics);

private slots:
    void on_pushButton_clicked();

private:
    Ui::DynamicsWindow *ui;
};

#endif // DYNAMICSWINDOW_H
