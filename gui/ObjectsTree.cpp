#include <QMouseEvent>
#include "ObjectsTree.h"

ObjectsTree::ObjectsTree(QObject *parent) :
    QTreeWidget()
{
}

void ObjectsTree::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);

    if (itemAt(event->pos()) == NULL) {
        this->clearSelection();
        deselectedAll();
    }
}
