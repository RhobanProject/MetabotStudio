#ifndef PARAMETERSEDITOR_H
#define PARAMETERSEDITOR_H

#include <QDialog>

namespace Ui {
class ParametersEditor;
}

class ParametersEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersEditor(QWidget *parent = 0);
    ~ParametersEditor();

private:
    Ui::ParametersEditor *ui;
};

#endif // PARAMETERSEDITOR_H
