#ifndef PARAMETERSEDITOR_H
#define PARAMETERSEDITOR_H

#include <vector>
#include <QDialog>
#include <ParameterWidget.h>
#include <metabot/Robot.h>
#include "ValueWidget.h"

namespace Ui {
class ParametersEditor;
}

class MainWindow;
class ParametersEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersEditor(MainWindow *main, Metabot::Robot *robot, QWidget *parent = 0);
    ~ParametersEditor();

    void addValue(ValueWidget *value);
    void update();

private slots:
    void on_ok_clicked();
    void on_add_clicked();
    void on_generate_clicked();

private:
    MainWindow *main;
    Ui::ParametersEditor *ui;
    Metabot::Robot *robot;
    std::vector<ValueWidget*> values;
};

#endif // PARAMETERSEDITOR_H
