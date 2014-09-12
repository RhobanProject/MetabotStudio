#include <iostream>
#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include <metabot/AnchorPoint.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    robot(NULL), viewer(NULL), wizard(NULL)
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

    /////// SIMPLE TEST //////
    /*
    Metabot::ComponentInstance *body = backend->getComponent("body")->instanciate();
    body->compile();
    robot->root = body;
    viewer->updateRatio();

    // Component wizard dialog
    wizard = new ComponentWizard(viewer, robot, body->anchors[1]);
    wizard->show();
    */
    /////// /SIMPLE TEST //////

    drawTree();
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
                QString label = QString("anchor %1").arg(index);
                item->setText(0, label);
                item->setTextColor(0, QColor("#aaa"));
            } else {
                Metabot::ComponentInstance *subInstance = anchor->anchor->instance;
                item->setText(0, QString::fromStdString(subInstance->component->name));
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
        item->setText(0, QString::fromStdString(robot->root->component->name));
    }
    items[item] = NULL;
    ui->tree->addTopLevelItem(item);

    if (robot->root) {
        drawTreeRecursive(item, robot->root);
        ui->tree->expandAll();
    }
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}

void MainWindow::on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->tree->blockSignals(true);

    if (wizard != NULL) {
        wizard->cancel();
        delete wizard;
        wizard = NULL;
    }

    if (items.count(item)) {
        Metabot::AnchorPoint *anchor = items[item];
        wizard = new ComponentWizard(viewer, robot, anchor);
        wizard->show();
        QObject::connect(wizard, SIGNAL(on_ok()), this, SLOT(on_wizard_ok()));
        QObject::connect(wizard, SIGNAL(on_cancel()), this, SLOT(on_wizard_cancel()));
    }

    ui->tree->blockSignals(false);
}

void MainWindow::on_wizard_ok()
{
    drawTree();
    wizard->close();
    delete wizard;
    wizard = NULL;
}

void MainWindow::on_wizard_cancel()
{
    delete wizard;
    wizard = NULL;
}
