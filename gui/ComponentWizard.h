#ifndef COMPONENTWIZARD_H
#define COMPONENTWIZARD_H

#include <QDialog>
#include <metabot/Backend.h>
#include <QListWidgetItem>
#include "Viewer.h"

namespace Ui {
class ComponentWizard;
}

class ComponentWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit ComponentWizard(QWidget *parent = 0);
    ~ComponentWizard();

    void setBackend(Metabot::Backend *backend);

    Metabot::Backend *backend;
    
private slots:
    void on_ok_clicked();

    void on_listWidget_itemSelectionChanged();

private:
    Ui::ComponentWizard *ui;
    Viewer *viewer;
    Metabot::Model model;
    Metabot::ComponentInstance *instance;
};

#endif // COMPONENTWIZARD_H
