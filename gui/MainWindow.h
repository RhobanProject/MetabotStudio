#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QTimer>
#include <QMenu>
#include <QSettings>
#include <QPoint>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include "Viewer.h"
#include <metabot/Backend.h>
#include <metabot/Robot.h>
#include "ComponentWizard.h"
#include "ZerosEditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void connectItem(QTreeWidgetItem *item, Metabot::AnchorPoint *anchor);
    void drawTree();
    void drawTreeRecursive(QTreeWidgetItem *parentItem, Metabot::ComponentInstance *instance);

    void runWizard(QTreeWidgetItem *item);

public slots:
    void on_wizard_ok();
    void on_wizard_cancel();

private slots:

    void on_wizard_clicked();

    void on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_tree_itemSelected(QTreeWidgetItem*,QTreeWidgetItem*);
    void on_tree_itemDeselected();

    void on_contextmenu_request(QPoint pt);
    void on_contextmenu_add();
    void on_contextmenu_edit();
    void on_contextmenu_remove();

    void on_clicked();
    void on_close();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionQuit_triggered();

    void on_actionZeros_triggered();

private:
    QSettings settings;
    QTreeWidgetItem *contextmenu_item;
    QAction addComponent, editComponent, removeComponent;
    QMenu menu;
    Metabot::Robot *robot;
    Viewer *viewer;
    Ui::MainWindow *ui;
    Metabot::Backend *backend;
    ComponentWizard *wizard;
    std::map<QTreeWidgetItem*, Metabot::AnchorPoint*> items;
    QString filename;
    ZerosEditor *zeros;
};

#endif // MAINWINDOW_H
