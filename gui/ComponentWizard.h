#ifndef COMPONENTWIZARD_H
#define COMPONENTWIZARD_H

#include <QDialog>
#include <metabot/Backend.h>

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
    
private:
    Ui::ComponentWizard *ui;
};

#endif // COMPONENTWIZARD_H
