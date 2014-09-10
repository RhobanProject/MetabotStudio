#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#include <QWidget>

namespace Ui {
class ComponentItem;
}

class ComponentItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComponentItem(QWidget *parent = 0);
    ~ComponentItem();

    void setInfo(std::string name, std::string description);
    
private:
    Ui::ComponentItem *ui;
};

#endif // COMPONENTITEM_H
