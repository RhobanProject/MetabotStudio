#ifndef COMPONENTWIZARD_H
#define COMPONENTWIZARD_H

#include <QDialog>

namespace Ui {
class ComponentWizard;
}

class ComponentWizard : public QDialog
{
    Q_OBJECT
    
public:
    explicit ComponentWizard(QWidget *parent = 0);
    ~ComponentWizard();
    
private:
    Ui::ComponentWizard *ui;
};

#endif // COMPONENTWIZARD_H
