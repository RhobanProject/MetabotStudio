#include "ParametersEditor.h"
#include "ui_ParametersEditor.h"
#include "Parameters.h"
#include "MainWindow.h"

ParametersEditor::ParametersEditor(MainWindow *main_, Metabot::Robot *robot_, QWidget *parent) :
    main(main_), robot(robot_), QDialog(parent),
    ui(new Ui::ParametersEditor)
{
    ui->setupUi(this);
    setWindowTitle("Parameters");

    for (auto entry : robot->parameters) {
        auto value = new ValueWidget(this);
        value->set(QString::fromStdString(entry.first), entry.second);
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
    robot->parameters = Metabot::Parameters();
    for (auto value : values) {
        auto key = value->getKey();
        auto p = value->getValue();
        if (key != "") {
            robot->parameters.set(key.toStdString(), p);
        }
    }
}

void ParametersEditor::on_ok_clicked()
{
    update();
    robot->compile();
    robot->number();
    main->drawTree();
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
    main->drawTree();
    main->getViewer()->redraw();
}
