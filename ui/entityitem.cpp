#include "entityitem.h"

EntityItem::EntityItem(const QString& entityTypeName, Entity3D* entity)
{
    this->init(entityTypeName, entity);
}


EntityItem::EntityItem(const QString& entityTypeName, Entity3D* entity, QTreeWidget *parent):
    QTreeWidgetItem(parent)
{
    this->init(entityTypeName, entity);
}

EntityItem::EntityItem(const QString& entityTypeName, Entity3D* entity, QTreeWidgetItem *parent):
    QTreeWidgetItem(parent)
{
    this->init(entityTypeName, entity);
}

void EntityItem::init(const QString& entityTypeName, Entity3D* entity)
{
    this->entityTypeName = entityTypeName;
    this->entity = entity;

    this->setText(0, entityTypeName);
    this->setExpanded(true);
}
