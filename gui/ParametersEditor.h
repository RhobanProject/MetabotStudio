#ifndef PARAMETERSEDITOR_H
#define PARAMETERSEDITOR_H

#include <vector>
#include <QDialog>
#include <ParameterWidget.h>
#include <metabot/Robot.h>

namespace Ui {
class ParametersEditor;
}

class ParametersEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersEditor(Metabot::Robot *robot, QWidget *parent = 0);
    ~ParametersEditor();

private slots:
    void on_ok_clicked();

    void on_add_clicked();

    void on_generate_clicked();

private:
    Ui::ParametersEditor *ui;
    Metabot::Robot *robot;
    std::vector<ParameterWidget*> parameters;
};

#endif // PARAMETERSEDITOR_H
