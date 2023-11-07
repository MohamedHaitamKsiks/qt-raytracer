#ifndef ENTITYITEM_H
#define ENTITYITEM_H

#include <QTreeWidgetItem>
#include "entity3d/entity3d.h"


class EntityItem : public QTreeWidgetItem
{
public:
    EntityItem(const QString& entityTypeName, Entity3D* entity);
    EntityItem(const QString& entityTypeName, Entity3D* entity, QTreeWidget *parent);
    EntityItem(const QString& entityTypeName, Entity3D* entity, QTreeWidgetItem *parent);

    inline Entity3D* getEntity()
    {
        return this->entity;
    }

    inline const QString& getEntityTypeName() const
    {
        return this->entityTypeName;
    }

private:
    Entity3D* entity = nullptr;
    QString entityTypeName;

    // init values
    void init(const QString& entityTypeName, Entity3D* entity);
};

#endif // ENTITYITEM_H
