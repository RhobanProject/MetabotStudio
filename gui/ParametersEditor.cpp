#include "ParametersEditor.h"
#include "ui_ParametersEditor.h"

ParametersEditor::ParametersEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersEditor)
{
    ui->setupUi(this);
}

ParametersEditor::~ParametersEditor()
{
    delete ui;
}
