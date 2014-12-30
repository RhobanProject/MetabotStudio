#ifndef ZEROSEDITOR_H
#define ZEROSEDITOR_H

#include <vector>
#include <QSlider>
#include <QDialog>
#include <metabot/Robot.h>
#include "Viewer.h"

namespace Ui {
    class ZerosEditor;
}

class MainWindow;

class ZerosEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit ZerosEditor(MainWindow *main, Metabot::Robot *robot, Viewer *viewer, QWidget *parent = 0);
    ~ZerosEditor();

    void init();
    void close();

public slots:
    void changed();
    
private slots:
    void on_instance_clicked(Metabot::Component *instance);
    void on_ok_clicked();
    void on_reset_clicked();
    void on_cancel_clicked();
    void on_zero_clicked();
    void focus();

private:
    MainWindow *main;
    Ui::ZerosEditor *ui;
    Metabot::Robot *robot;
    Viewer *viewer;
    std::vector<QSlider*> sliders;
    std::vector<float> previous;
};

#endif // ZEROSEDITOR_H
