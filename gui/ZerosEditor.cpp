#include <iostream>
#include <math.h>
#include "ZerosEditor.h"
#include "ui_ZerosEditor.h"
#include "MainWindow.h"
#include <metabot/util/util.h>

ZerosEditor::ZerosEditor(MainWindow *main_, Metabot::Robot *robot_, Viewer *viewer_, QWidget *parent)
    : main(main_),
      robot(robot_),
      viewer(viewer_),
    QDialog(parent),
    ui(new Ui::ZerosEditor)
{
    ui->setupUi(this);

    previous = robot->getZeros();
    setWindowTitle("Zeros editor");

    QObject::connect(viewer, SIGNAL(component_clicked(Metabot::Component*)), this, SLOT(on_instance_clicked(Metabot::Component*)));

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
            auto anchors = robot->getAnchors();
            if (i < anchors.size()) {
                main->highlightAnchor(anchors[i]);
            }
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
    viewer->redraw();
}

void ZerosEditor::on_instance_clicked(Metabot::Component *instance)
{
    auto anchor = instance->aboveAnchor();
    if (anchor) {
        auto anchors = robot->getAnchors();
        int n = 0;
        for (auto robotAnchor : anchors) {
            if (robotAnchor == anchor) {
                if (n < sliders.size()) {
                    this->activateWindow();
                    this->setFocus();
                    QSlider *slider = sliders[n];
                    slider->setFocus();
                }
            }
            n++;
        }
    }
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
