#ifndef MESHINSTANCE3D_H
#define MESHINSTANCE3D_H

#include "entity3d.h"

class MeshInstance3D: public Entity3D
{
public:
    void serializeFields(QLayout* layout);

    MeshInstance3D() {};
    MeshInstance3D(Entity3D* parent): Entity3D(parent){}

private:
    // mesh data
    QString meshPath = "suzanne.obj";

    void draw();
};

#endif // MESHINSTANCE3D_H
