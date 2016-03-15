#include <iostream>
#include "BackendSelector.h"
#include "ui_BackendSelector.h"
#include <QDir>
#include <util/util.h>
#include <QProcess>

BackendSelector::BackendSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BackendSelector)
{
    ui->setupUi(this);

    QDir dir(QDir::currentPath()+"/backends");
    QFileInfoList info = dir.entryInfoList();
    foreach (QFileInfo file, info) {
        if (file.isDir() && file.fileName()!="." && file.fileName()!="..") {
            ui->backends->addItem(file.fileName());
        }
    }
}

BackendSelector::~BackendSelector()
{
    delete ui;
}

void BackendSelector::on_buttonBox_accepted()
{
    emit selectedBackend(ui->backends->currentText());
}
