#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QTimer>
#include <QMainWindow>
#include <metabot/Backend.h>
#include "ComponentWizard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    
private slots:

    void on_wizard_clicked();

private:
    Ui::MainWindow *ui;
    Metabot::Backend *backend;
    ComponentWizard *wizard;
};

#endif // MAINWINDOW_H
