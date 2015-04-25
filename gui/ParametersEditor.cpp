#include "ParametersEditor.h"
#include "ui_ParametersEditor.h"

ParametersEditor::ParametersEditor(Metabot::Robot *robot_, QWidget *parent) :
    robot(robot_), QDialog(parent),
    ui(new Ui::ParametersEditor)
{
    ui->setupUi(this);
}

ParametersEditor::~ParametersEditor()
{
    for (auto parameter : parameters) {
        delete parameter;
    }

    delete ui;
}

void ParametersEditor::on_ok_clicked()
{
    this->close();
}

void ParametersEditor::on_add_clicked()
{

}

void ParametersEditor::on_generate_clicked()
{

}
