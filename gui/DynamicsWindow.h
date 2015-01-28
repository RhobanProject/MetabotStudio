#ifndef DYNAMICSWINDOW_H
#define DYNAMICSWINDOW_H

#include <QDialog>

namespace Ui {
class DynamicsWindow;
}

class DynamicsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DynamicsWindow(QWidget *parent = 0);
    ~DynamicsWindow();

private:
    Ui::DynamicsWindow *ui;
};

#endif // DYNAMICSWINDOW_H
