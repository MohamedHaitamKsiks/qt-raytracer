#include "entity3d.h"
#include <QTransform>
#include <QVBoxLayout>


Entity3D::Entity3D()
{

}

Entity3D::Entity3D(Entity3D* parent)
{
    if (!parent) return;

    parent->addChild(this);
}

Entity3D::~Entity3D()
{
    if (this->parent)
        this->parent->children.removeAll(this);

    for (auto* entity: this->children)
    {
        delete entity;
    }
}

void Entity3D::serializeFields(QLayout* layout)
{
    // transform
    FieldSerializer::SerializeLayout("Local Position", this->localPosition, layout);

    // material
    FieldSerializer::SerializeLayout("Material", this->material, layout);
}

Entity3D* Entity3D::addChild(Entity3D* entity)
{
    entity->parent = this;
    this->children.append(entity);
    return entity;
}

QMatrix4x4 Entity3D::getLocalTransform()
{
    QMatrix4x4 rotation{};
    rotation.rotate(localRotation);

    QMatrix4x4 scale{};
    scale.scale(localScale);

    QMatrix4x4 translate{};
    translate.translate(localPosition);

    return translate * scale * rotation;
}

void Entity3D::update()
{
    // update transform
    if (this->parent)
        this->globalTransfrom = this->parent->getGlobalTransform() * getLocalTransform();
    else
        this->globalTransfrom = getLocalTransform();

    // draw entity
    this->draw();

    // udpate children
    for (auto* entity: this->children)
    {
        entity->update();
    }
}


