#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QThread>
#include <QTimer>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include "Viewer.h"
#include <metabot/Backend.h>
#include <metabot/Robot.h>
#include "ComponentWizard.h"

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

public slots:
    void on_wizard_ok();
    void on_wizard_cancel();

private slots:

    void on_wizard_clicked();

    void on_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Metabot::Robot *robot;
    Viewer *viewer;
    Ui::MainWindow *ui;
    Metabot::Backend *backend;
    ComponentWizard *wizard;
    std::map<QTreeWidgetItem*, Metabot::AnchorPoint*> items;
};

#endif // MAINWINDOW_H
