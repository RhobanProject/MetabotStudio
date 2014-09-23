#include <iostream>
#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <metabot/AnchorPoint.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("Rhoban", "Metabot"),
    addComponent("Add component", this),
    editComponent("Edit", this),
    removeComponent("Remove", this),
    robot(NULL), wizard(NULL),
    filename(""),
    zeros(NULL)
{
    ui->setupUi(this);
    setWindowTitle("Metabot");

    // Loading metabot backend
    backend = new Metabot::Backend("xl-320");
    backend->load();

    // Creating viewer
    viewer = new Viewer();
    ui->viewer->addWidget(viewer);

    // Creating robot
    robot = new Metabot::Robot(backend);
    viewer->setRobot(robot);

     /*
    // Debugging auto-open
    filename = "/home/gregwar/Metabot/robots/spidey12.robot";
    robot->loadFromFile(filename.toStdString());
    ui->actionSave->setEnabled(true);
     */

    viewer->updateRatio();

    QList<int> sizes;
    sizes.append(150);
    sizes.append(400);
    ui->splitter->setSizes(sizes);

    // Tree
    ui->tree->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->tree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_contextmenu_request(QPoint)));
    QObject::connect(&addComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_add()));
    QObject::connect(&editComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_edit()));
    QObject::connect(&removeComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_remove()));
    QObject::connect(ui->tree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(on_tree_itemSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
    QObject::connect(ui->tree, SIGNAL(deselectedAll()), this, SLOT(on_tree_itemDeselected()));
    drawTree();

    ui->actionGrid->setChecked(true);
    ui->actionXYZ->setChecked(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::drawTreeRecursive(QTreeWidgetItem *parentItem,
                                   Metabot::ComponentInstance *instance
                                   )
{
    int index = 1;
    std::vector<Metabot::AnchorPoint *>::iterator it;
    for (it=instance->anchors.begin(); it!=instance->anchors.end(); it++) {
        Metabot::AnchorPoint *anchor = *it;
        if (anchor->type != "root" && anchor->above) {
            QTreeWidgetItem *item = new QTreeWidgetItem;
            items[item] = anchor;
            if (anchor->anchor == NULL) {
                QString label = QString("%1 #%2").arg(QString::fromStdString(anchor->type)).arg(index);
                item->setText(0, label);
                item->setTextColor(0, QColor("#aaa"));
            } else {
                Metabot::ComponentInstance *subInstance = anchor->anchor->instance;
                item->setText(0, QString::fromStdString(subInstance->fullName()));
                drawTreeRecursive(item, subInstance);
            }
            parentItem->addChild(item);
            index++;
        }
    }
}

void MainWindow::drawTree()
{
    ui->tree->clear();
    items.clear();

    // Adding root item
    QTreeWidgetItem *item = new QTreeWidgetItem;

    if (robot->root == NULL) {
        item->setText(0, "root");
        item->setTextColor(0, QColor("#aaa"));
    } else {
        item->setText(0, QString::fromStdString(robot->root->fullName()));
    }
    items[item] = NULL;
    ui->tree->addTopLevelItem(item);

    if (robot->root) {
        drawTreeRecursive(item, robot->root);
        ui->tree->expandAll();
    }

    robot->unHighlight();
    viewer->updatePlate();
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}

void MainWindow::runWizard(QTreeWidgetItem *item)
{
    if (wizard != NULL) {
        wizard->cancel();
        delete wizard;
        wizard = NULL;
    }

    if (items.count(item)) {
        Metabot::AnchorPoint *anchor = items[item];
        wizard = new ComponentWizard(viewer, robot, anchor);
        wizard->show();
        wizard->restoreGeometry(settings.value("componentsWizard").toByteArray());
        wizard->adjustSize();

        QObject::connect(wizard, SIGNAL(on_ok()), this, SLOT(on_wizard_ok()));
        QObject::connect(wizard, SIGNAL(on_cancel()), this, SLOT(on_wizard_cancel()));
    }
}

void MainWindow::on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->tree->blockSignals(true);

    runWizard(item);

    ui->tree->blockSignals(false);
}

void MainWindow::on_tree_itemSelected(QTreeWidgetItem *item, QTreeWidgetItem *)
{
    robot->unHighlight();
    if (item != NULL && items.count(item)) {
        Metabot::AnchorPoint *anchor = items[item];

        if (anchor != NULL) {
           anchor->highlight = true;
        }
    }
}

void MainWindow::on_tree_itemDeselected()
{
    robot->unHighlight();
}

void MainWindow::on_contextmenu_request(QPoint pt)
{
    QTreeWidgetItem *item = ui->tree->itemAt(pt);
    if (item != NULL) {
        QPoint pos = ui->tree->mapToGlobal(pt);
        QPoint pos2(pos.x(), pos.y());
        Metabot::AnchorPoint *anchor = items[item];
        menu.clear();
        if ((anchor == NULL && robot->root == NULL) || (anchor != NULL && anchor->anchor == NULL)) {
            menu.addAction(&addComponent);
        } else {
            menu.addAction(&editComponent);
            menu.addAction(&removeComponent);
        }
        contextmenu_item = item;
        menu.exec(pos2);
    }
}


void MainWindow::on_close()
{
    if (wizard != NULL) {
        wizard->close();
    }
}

void MainWindow::on_wizard_ok()
{
    drawTree();
    settings.setValue("componentsWizard", wizard->saveGeometry());
    wizard->close();
    delete wizard;
    wizard = NULL;
}

void MainWindow::on_wizard_cancel()
{
    settings.setValue("componentsWizard", wizard->saveGeometry());
    delete wizard;
    wizard = NULL;
}

void MainWindow::on_contextmenu_add()
{
    runWizard(contextmenu_item);
}

void MainWindow::on_contextmenu_remove()
{
    Metabot::AnchorPoint *anchor = items[contextmenu_item];
    if (anchor != NULL) {
        anchor->detach();
    } else {
        robot->root = NULL;
    }
    drawTree();
}

void MainWindow::on_clicked()
{
}

void MainWindow::on_contextmenu_edit()
{
    runWizard(contextmenu_item);
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog;
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter("Robot (*.robot)");
    if (dialog.exec()) {
        QString fn = dialog.selectedFiles().first();
        if (fn != "") {
            ui->actionSave->setEnabled(true);
            filename = fn;
            robot->clear();
            try {
                robot->loadFromFile(fn.toStdString());
            } catch (std::string error) {
                QMessageBox::warning(this,"Erreur de chargement", QString::fromStdString(error));
            }
            drawTree();
            viewer->updateRatio();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (filename != "") {
        robot->saveToFile(filename.toStdString());
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dialog;
    dialog.setNameFilter("Robot (*.robot)");
    dialog.setDefaultSuffix("robot");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec()) {
        QString fn = dialog.selectedFiles().first();
        if (fn != "") {
            ui->actionSave->setEnabled(true);
            filename = fn;
            robot->saveToFile(filename.toStdString());
        }
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionZeros_triggered()
{
    if (zeros != NULL) {
        zeros->close();
        delete zeros;
    }
    zeros = new ZerosEditor(robot, viewer);
    zeros->show();
    robot->unHighlight();
    ui->tree->clearSelection();
}

void MainWindow::on_actionReset_view_triggered()
{
    if (viewer) {
        viewer->updateRatio();
    }
}

void MainWindow::on_actionGrid_toggled(bool drawGrid)
{
    viewer->drawGrid = drawGrid;
}

void MainWindow::on_actionXYZ_toggled(bool drawXYZ)
{
    viewer->drawXYZ = drawXYZ;
}
