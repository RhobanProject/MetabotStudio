#ifndef OBJECTSTREE_H
#define OBJECTSTREE_H

#include <QTreeWidget>

class ObjectsTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit ObjectsTree(QObject *parent = 0);

    void mousePressEvent(QMouseEvent * event);
    
signals:
    void deselectedAll();
    
public slots:
    
};

#endif // OBJECTSTREE_H
