#ifndef COMPONENTWIZARD_H
#define COMPONENTWIZARD_H

#include <QDialog>
#include <metabot/Backend.h>
#include <QListWidgetItem>
#include <QRadioButton>
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
    void setAnchor(Metabot::AnchorPoint *anchor);
    void setupInstance();

    Metabot::Backend *backend;
    void fill();
    Viewer *viewer;
    
private slots:
    void on_listWidget_itemSelectionChanged();

    void on_anchorPoint_clicked();

private:
    std::vector<QRadioButton*> anchorButtons;
    std::map<QRadioButton*, Metabot::AnchorPoint*> buttonToAnchor;
    Ui::ComponentWizard *ui;
    Metabot::Model model;
    Metabot::ComponentInstance *instance;
};

#endif // COMPONENTWIZARD_H
