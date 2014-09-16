#include <math.h>
#include "ZerosEditor.h"
#include "ui_ZerosEditor.h"
#include <metabot/util.h>

ZerosEditor::ZerosEditor(Metabot::Robot *robot_, Viewer *viewer_, QWidget *parent)
    : robot(robot_),
      viewer(viewer_),
    QDialog(parent),
    ui(new Ui::ZerosEditor)
{
    ui->setupUi(this);

    previous = robot->getZeros();
    setWindowTitle("Zeros editor");

    init();
}

ZerosEditor::~ZerosEditor()
{
    delete ui;
}

void ZerosEditor::init()
{
    for (int i=0; i<sliders.size(); i++) {
        ui->sliders->removeWidget(sliders[i]);
        delete sliders[i];
    }
    sliders.clear();

    for (int i=0; i<previous.size(); i++) {
        float angle = RAD2DEG(previous[i]);
        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setMinimum(-1800);
        slider->setMaximum(1800);
        slider->setValue(10*angle);
        QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changed()));
        QObject::connect(slider, SIGNAL(sliderPressed()), this, SLOT(focus()));
        sliders.push_back(slider);
        ui->sliders->addWidget(slider);
    }

    adjustSize();
}

void ZerosEditor::close()
{
    robot->unHighlight();
    QDialog::close();
}

void ZerosEditor::focus()
{
    for (int i=0; i<sliders.size(); i++) {
        QSlider *slider = sliders[i];
        if (slider == sender()) {
            robot->highlightNth(i);
        }
    }
}

void ZerosEditor::changed()
{
    std::vector<float> zeros;
    for (int i=0; i<sliders.size(); i++) {
        QSlider *slider = sliders[i];
        float angle = DEG2RAD(slider->value()/10.0);
        zeros.push_back(angle);
    }
    robot->setZeros(zeros);
}

void ZerosEditor::on_ok_clicked()
{
    changed();
    close();
    viewer->updatePlate();
}

void ZerosEditor::on_reset_clicked()
{
    init();
    changed();
}

void ZerosEditor::on_cancel_clicked()
{
    robot->setZeros(previous);
    close();
}

void ZerosEditor::on_zero_clicked()
{
    for (int i=0; i<sliders.size(); i++) {
        QSlider *slider = sliders[i];
        slider->setValue(0);
    }
    changed();
}
