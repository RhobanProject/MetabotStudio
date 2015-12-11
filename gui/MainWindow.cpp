#include <iostream>
#include "MainWindow.h"
#include "ComponentWizard.h"
#include "ui_MainWindow.h"
#include "ZerosEditor.h"
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <metabot/AnchorPoint.h>
#include <metabot/3d/stl.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("Rhoban", "Metabot"),
    addComponent("Add component", this),
    editComponent("Edit", this),
    removeComponent("Remove", this),
    rootComponent("Root", this),
    centerComponent("Center", this),
    dynamicsComponent("Dynamics", this),
    robot(NULL), robotSave(NULL),
    wizard(NULL), dynamics(NULL), parametersEditor(NULL),
    filename(""),
    zeros(NULL)
{

    ui->setupUi(this);
    setWindowTitle("Metabot");

    // Loading metabot backend
    backend = new Metabot::Backend("xl-320");
    // backend = new Metabot::Backend("abstract");
    backend->load();

    // Creating viewer
    viewer = new Viewer();
    ui->viewer->addWidget(viewer);

    // Creating robot
    robot = new Metabot::Robot(backend);
    viewer->setRobot(robot);

    // Debugging auto-open
    // filename = "/home/gregwar/Metabot/robots/remi.robot";
    filename = "/home/gregwar/Metabot/robots/metabot.robot";
    robot->loadFromFile(filename.toStdString());
    robot->number();
    robot->computeDynamics();
    // robot->toBullet();
    ui->actionSave->setEnabled(true);

    // Viewer
    QObject::connect(viewer, SIGNAL(autorotate_changed(bool)), this, SLOT(on_viewer_autorotate_change(bool)));
    QObject::connect(viewer, SIGNAL(anchor_clicked(Metabot::AnchorPoint*)), this, SLOT(on_viewer_anchor_clicked(Metabot::AnchorPoint*)));
    QObject::connect(viewer, SIGNAL(component_clicked(Metabot::Component*)), this, SLOT(on_viewer_clicked(Metabot::Component*)));
    QObject::connect(viewer, SIGNAL(component_double_clicked(Metabot::Component*)), this, SLOT(on_viewer_doubleclicked(Metabot::Component*)));
    QObject::connect(viewer, SIGNAL(nowhere_clicked()), this, SLOT(on_viewer_nowhere_clicked()));
    viewer->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(viewer, SIGNAL(viewer_contextmenu_request(QPoint)), this, SLOT(on_viewer_contextmenu_request(QPoint)));
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
    QObject::connect(&rootComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_root()));
    QObject::connect(&centerComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_center()));
    QObject::connect(&dynamicsComponent, SIGNAL(triggered()), this, SLOT(on_contextmenu_dynamics()));
    QObject::connect(ui->tree, SIGNAL(itemSelectionChanged()), this, SLOT(on_tree_itemSelectionChanged()));
    QObject::connect(ui->tree, SIGNAL(deselectedAll()), this, SLOT(on_tree_itemDeselected()));
    drawTree();

    ui->actionGrid->setChecked(true);
    ui->actionXYZ->setChecked(true);

    on_action_tree_triggered();
}

MainWindow::~MainWindow()
{
}

void MainWindow::drawTreeRecursive(QTreeWidgetItem *parentItem,
                                   Metabot::Component *instance
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
                Metabot::Component *subInstance = anchor->anchor->component;
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
        item->setTextColor(0,    QColor("#aaa"));
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
    ui->tree->clearSelection();
}

void MainWindow::on_wizard_clicked()
{
    wizard->show();
}

void MainWindow::runWizard(Metabot::AnchorPoint *anchor)
{
    if (zeros) {
        zeros->close();
        delete zeros;
        zeros = NULL;
    }

    if (wizard != NULL) {
        wizard->activateWindow();
    } else {
        robotSave = robot->clone();
        wizard = new ComponentWizard(viewer, robot, anchor);
        wizard->show();
        wizard->restoreGeometry(settings.value("componentsWizard").toByteArray());

        QObject::connect(wizard, SIGNAL(on_ok()), this, SLOT(on_wizard_ok()));
        QObject::connect(wizard, SIGNAL(on_cancel()), this, SLOT(on_wizard_cancel()));
    }
}

void MainWindow::on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    ui->tree->blockSignals(true);

    if (items.count(item)) {
        runWizard(items[item]);
    }

    ui->tree->blockSignals(false);
}

void MainWindow::on_tree_itemSelectionChanged()
{
    robot->unHighlight();
    auto selectedItems = ui->tree->selectedItems();
    if (selectedItems.count() == 1) {
        QTreeWidgetItem *item = selectedItems.first();
        if (item != NULL && items.count(item)) {
            Metabot::AnchorPoint *anchor = items[item];
            highlightAnchor(anchor);
        }
    }
    viewer->redraw();
}

void MainWindow::highlightAnchor(Metabot::AnchorPoint *anchor)
{
    robot->unHighlight();

    if (anchor != NULL) {
        anchor->highlight = true;
        if (anchor->anchor) {
            anchor->anchor->component->highlight = true;
        }
    } else {
        if (robot->root != NULL) {
            robot->root->highlight = true;
        }
    }
}

void MainWindow::deselectAll()
{
    robot->unHighlight();
    ui->tree->clearSelection();
}

Viewer *MainWindow::getViewer()
{
    return viewer;
}

void MainWindow::on_tree_itemDeselected()
{
    robot->unHighlight();
}

void MainWindow::on_viewer_contextmenu_request(QPoint pt)
{
    auto items = ui->tree->selectedItems();

    auto instance = viewer->getInstanceAt(pt.x(), pt.y());
    if (instance) {
        on_viewer_clicked(instance);
    }

    if (items.size()) {
        showContextMenu(viewer->mapToGlobal(pt), items[0]);
    } else {
        if (robot->root == NULL) {
            showContextMenu(viewer->mapToGlobal(pt), NULL);
        }
    }
}

void MainWindow::on_contextmenu_request(QPoint pt)
{
    QTreeWidgetItem *item = ui->tree->itemAt(pt);
    if (item != NULL) {
        QPoint pos = ui->tree->mapToGlobal(pt);
        showContextMenu(pos, item);
    }
}

void MainWindow::showContextMenu(QPoint pos, QTreeWidgetItem *item)
{
    Metabot::AnchorPoint *anchor = item==NULL ? NULL : items[item];
    menu.clear();
    if ((anchor == NULL && robot->root == NULL)
     || (anchor != NULL && anchor->anchor == NULL)) {
        menu.addAction(&addComponent);
    } else {
        menu.addAction(&editComponent);
        menu.addAction(&removeComponent);
        menu.addAction(&rootComponent);
        menu.addAction(&centerComponent);
        menu.addAction(&dynamicsComponent);
    }
    contextmenu_item = item;
    menu.exec(pos);
}

void MainWindow::showDynamicsWindow(Dynamics dyn)
{
    if (dynamics != NULL) {
        dynamics->close();
        delete dynamics;
    }
    dynamics = new DynamicsWindow(this);
    dynamics->setDynamics(dyn);
    dynamics->show();
}

void MainWindow::on_close()
{
    if (wizard != NULL) {
        wizard->close();
    }
}

void MainWindow::on_wizard_ok()
{
    robot->number();
    drawTree();
    settings.setValue("componentsWizard", wizard->saveGeometry());
    wizard->close();
    delete wizard;
    delete robotSave;
    wizard = NULL;
}

void MainWindow::on_wizard_cancel()
{
    delete robot;
    robot = robotSave;
    robot->number();
    viewer->robot = robot;
    drawTree();
    settings.setValue("componentsWizard", wizard->saveGeometry());
    delete wizard;
    wizard = NULL;
}

void MainWindow::on_viewer_clicked(Metabot::Component *instance)
{
    // Highlight it
    Metabot::AnchorPoint *anchor = instance->aboveAnchor();
    highlightAnchor(anchor);

    // Change the selection in the tree
    for (auto entry : items) {
        if (entry.second == anchor) {
            ui->tree->clearSelection();
            ui->tree->setItemSelected(entry.first, true);
        }
    }
}

void MainWindow::on_viewer_doubleclicked(Metabot::Component *instance)
{
    Metabot::AnchorPoint *anchor = instance->aboveAnchor();
    runWizard(anchor);
}

void MainWindow::on_viewer_anchor_clicked(Metabot::AnchorPoint *anchor)
{
    runWizard(anchor);
}

void MainWindow::on_viewer_nowhere_clicked()
{
    deselectAll();
}

void MainWindow::on_viewer_autorotate_change(bool value)
{
    ui->actionAutorotate->setChecked(value);
}

void MainWindow::on_contextmenu_add()
{
    auto anchor = selectedAnchor();
    runWizard(anchor);
}

void MainWindow::on_contextmenu_remove()
{
    auto anchor = selectedAnchor();

    if (anchor != NULL) {
        anchor->detach();
    } else {
        robot->clear();
    }
    drawTree();
}

void MainWindow::on_contextmenu_root()
{
    auto anchor = selectedAnchor();

    if (anchor != NULL && anchor->anchor && anchor->anchor->component) {
        robot->setRoot(anchor->anchor->component);
        drawTree();
    }
}

void MainWindow::on_contextmenu_center()
{
    auto anchor = selectedAnchor();

    Metabot::Component *instance;
    if (anchor == NULL) {
        instance = robot->root;
    } else {
        instance = anchor->anchor->component;
    }
    Metabot::Vect v(0, 0, 0);
    v = robot->getPoint(instance, v);
    viewer->tX = -v.values[0];
    viewer->tY = -v.values[1];
}

void MainWindow::on_contextmenu_dynamics()
{
    auto component = selectedComponent();;
    if (component != NULL) {
        showDynamicsWindow(component->getDynamics());
    }
}

void MainWindow::on_clicked()
{
}

Metabot::AnchorPoint *MainWindow::selectedAnchor()
{
    if (contextmenu_item != NULL && items.count(contextmenu_item)) {
        return items[contextmenu_item];
    }

    return NULL;
}

Component *MainWindow::selectedComponent()
{
    auto anchor = selectedAnchor();
    if (anchor == NULL) {
        return robot->root;
    } else {
        return anchor->anchor->component;
    }
}

void MainWindow::on_contextmenu_edit()
{
    runWizard(selectedAnchor());
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
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
    QFileDialog dialog(this);
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
    zeros = new ZerosEditor(this, robot, viewer);
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

void MainWindow::on_actionAutorotate_triggered()
{
    viewer->setAutorotate(ui->actionAutorotate->isChecked());
}

void MainWindow::on_actionNew_triggered()
{
    robot->clear();
    drawTree();
}

void MainWindow::on_actionExport_STL_triggered()
{
    QFileDialog dialog(this);
    dialog.setNameFilter("STL (*.stl)");
    dialog.setDefaultSuffix("stl");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec()) {
        QString fn = dialog.selectedFiles().first();
        if (fn != "") {
            auto model = robot->toModel();
            saveModelToFileBinary(fn.toStdString().c_str(), &model);
        }
    }
}

void MainWindow::on_treeButton_clicked()
{
    if (ui->tree->isVisible()) {
        ui->tree->hide();
    } else {
        ui->tree->show();
    }
    ui->splitter->adjustSize();
}

void MainWindow::on_action_tree_triggered()
{
    int width = this->width();
    if (ui->action_tree->isChecked()) {
        QList<int> sizes;
        sizes.push_back(width*0.2);
        sizes.push_back(width*0.9);
        ui->splitter->setSizes(sizes);
    } else {
        QList<int> sizes;
        sizes.push_back(0);
        sizes.push_back(1);
        ui->splitter->setSizes(sizes);
    }
}

void MainWindow::on_actionCollisions_triggered()
{
    robot->setDrawCollisions(ui->actionCollisions->isChecked());
    viewer->redraw();
}

void MainWindow::on_actionExport_STLs_triggered()
{
    auto directory = QFileDialog::getExistingDirectory(this, "STLs print Export");
    if (directory != "") {
        robot->writeSTLs(directory.toStdString());
    }
}

void MainWindow::on_actionExport_URDF_triggered()
{
    auto directory = QFileDialog::getExistingDirectory(this, "URDF Export");
    if (directory != "") {
        robot->writeSDF(directory.toStdString());
    }
}

void MainWindow::on_actionCompute_dynamics_triggered()
{
    robot->computeDynamics();
    showDynamicsWindow(robot->getDynamics());
}

void MainWindow::on_actionCenter_of_mass_triggered()
{
    robot->setDrawCOM(ui->actionCenter_of_mass->isChecked());
    viewer->redraw();
}

void MainWindow::on_actionParameters_triggered()
{
    if (parametersEditor != NULL) {
        parametersEditor->close();
        delete parametersEditor;
    }
    parametersEditor = new ParametersEditor(this, robot, this);
    parametersEditor->show();
}

void MainWindow::on_actionPhysics_triggered()
{
    if (ui->actionPhysics->isChecked()) {
        viewer->mode = MODE_PHYSICS;
    } else {
        viewer->mode = MODE_NORMAL;
    }
    viewer->redraw();
}
