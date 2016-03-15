#ifndef BACKENDSELECTOR_H
#define BACKENDSELECTOR_H

#include <QDialog>

namespace Ui {
class BackendSelector;
}

class BackendSelector : public QDialog
{
    Q_OBJECT

public:
    explicit BackendSelector(QWidget *parent = 0);
    ~BackendSelector();

signals:
    void selectedBackend(QString backend);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BackendSelector *ui;
};

#endif // BACKENDSELECTOR_H
