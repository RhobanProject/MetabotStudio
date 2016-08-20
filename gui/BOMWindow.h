#ifndef BOMWINDOW_H
#define BOMWINDOW_H

#include <QDialog>

namespace Ui {
class BOMWindow;
}

class BOMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BOMWindow(QWidget *parent = 0);
    ~BOMWindow();

    void setBom(QString text);

private slots:
    void on_ok_clicked();

private:
    Ui::BOMWindow *ui;
};

#endif // BOMWINDOW_H
