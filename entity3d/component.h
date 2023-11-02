#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity3d.h"

class Component
{
public:
    Component(Entity3D* entity);

    virtual void draw() = 0;

private:
    Entity3D* owner = nullptr;
};

#endif // COMPONENT_H
