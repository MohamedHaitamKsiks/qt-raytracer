#ifndef BALL3D_H
#define BALL3D_H

#include <QColor>
#include "entity3d.h"

class Ball3D : public Entity3D
{
public:
    // ball radius
    float radius = 0.2f;

    // contructors
    Ball3D();
    Ball3D(Entity3D* parent): Entity3D(parent)
    {
    };

private:
    void draw();

};

#endif // BALL3D_H
