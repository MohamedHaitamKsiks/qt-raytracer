#ifndef CUBE3D_H
#define CUBE3D_H

#include "entity3d.h"

class Cube3D: public Entity3D
{
public:
    Cube3D() {};
    Cube3D(Entity3D* parent): Entity3D(parent) {};

    void serializeFields(QLayout* layout);

private:
    void draw();
    void loadCubeMesh();
};

#endif // CUBE3D_H
