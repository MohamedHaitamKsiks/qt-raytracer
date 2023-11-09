#ifndef ENTITY3D_H
#define ENTITY3D_H

#include <QVector>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QColor>
#include <QLayout>

#include "ui/fieldserializer.h"
#include "renderer/material.h"

// tree same as godot nodes
class Entity3D
{
public:
    // local transform
    QVector3D localPosition = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D localRotation = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D localScale = QVector3D(1.0f, 1.0f, 1.0f);

    // entity name
    QString name = "Entity3D";

    // material
    Material material{};

    // constructors
    Entity3D();
    Entity3D(Entity3D* parent);

    // destructor
    virtual ~Entity3D();

    // serialize fields
    virtual void serializeFields(QLayout* layout);

    // update
    void update();

    // draw
    virtual void draw() {};

    // get entity children
    inline const QVector<Entity3D*>& getChildren() const
    {
        return this->children;
    }

    // add new child
    Entity3D* addChild(Entity3D* entity);

    // get parent
    inline Entity3D* getParent()
    {
        return this->parent;
    }

    // get parent
    inline const Entity3D* getParent() const
    {
        return this->parent;
    }

    // get local transform matrix
    QMatrix4x4 getLocalTransform();


    // get global transform matrix
    inline const QMatrix4x4& getGlobalTransform()
    {
        return this->globalTransfrom;
    }

protected:
    // entity tree
    Entity3D* parent = nullptr;
    QVector<Entity3D*> children = {};

    // global transform
    QMatrix4x4 globalTransfrom;
};

#endif // ENTITY3D_H
