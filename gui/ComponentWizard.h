#ifndef COMPONENTWIZARD_H
#define COMPONENTWIZARD_H

#include <QDialog>
#include <metabot/Backend.h>
#include <QListWidgetItem>
#include <QRadioButton>
#include "ParameterWidget.h"
#include "Viewer.h"

namespace Ui {
class ComponentWizard;
}

class ComponentWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit ComponentWizard(Viewer *viewer, Metabot::Robot *robot, Metabot::AnchorPoint *anchor,
                             QWidget *parent = 0);
    ~ComponentWizard();

    // Set the anchor
    void setAnchor(Metabot::AnchorPoint *anchor);

    // Called to setup the UI for instance
    void setupInstance();

    // Fill the list of available components
    void fill();

    // Cancel
    void cancel();

signals:
    void on_ok();
    
private slots:
    void on_listWidget_itemSelectionChanged();

    void on_anchorPoint_clicked();

    void on_generate_clicked();

    void on_cancel_clicked();

    void on_ok_clicked();

private:
    // Robot and anchor we're working on
    Viewer *viewer;
    Metabot::Robot *robot;
    Metabot::AnchorPoint *anchor;

    // Candidate new instance and its anchor
    Metabot::ComponentInstance *instance;
    Metabot::ComponentInstance *previousInstance;
    Metabot::AnchorPoint *currentAnchor;

    // Widgets to make choice for anchor point and parameters
    std::vector<ParameterWidget*> parameters;
    std::map<QWidget*, Metabot::AnchorPoint*> buttonToAnchor;

    Ui::ComponentWizard *ui;
};

#endif // COMPONENTWIZARD_H
