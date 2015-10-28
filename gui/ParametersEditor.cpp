#include "ParametersEditor.h"
#include "ui_ParametersEditor.h"
#include "Values.h"
#include "MainWindow.h"

ParametersEditor::ParametersEditor(MainWindow *main_, Metabot::Robot *robot_, QWidget *parent) :
    main(main_), robot(robot_), QDialog(parent),
    ui(new Ui::ParametersEditor)
{
    ui->setupUi(this);

    for (auto entry : robot->parameters) {
        auto value = new ValueWidget(this);
        value->set(QString::fromStdString(entry.first), QString::fromStdString(entry.second));
        addValue(value);
    }
}

ParametersEditor::~ParametersEditor()
{
    for (auto value : values) {
        delete value;
    }

    delete ui;
}

void ParametersEditor::addValue(ValueWidget *value)
{
    values.push_back(value);
    ui->parameters->addWidget(value);
}

void ParametersEditor::update()
{
    robot->parameters = Metabot::Values();
    for (auto value : values) {
        auto key = value->getKey();
        auto v = value->getValue();
        if (key != "") {
            robot->parameters.set(key.toStdString(), v.toStdString());
        }
    }
}

void ParametersEditor::on_ok_clicked()
{
    update();
    robot->compile();
    robot->number();
    main->getViewer()->redraw();
    this->close();
}

void ParametersEditor::on_add_clicked()
{
    addValue(new ValueWidget);
}

void ParametersEditor::on_generate_clicked()
{
    update();
    robot->compile();
    robot->number();
    main->getViewer()->redraw();
}
