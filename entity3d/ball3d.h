#ifndef BALL3D_H
#define BALL3D_H

#include <QColor>
#include "entity3d.h"
#include "random.h"

class Ball3D : public Entity3D
{
public:
    // ball radius
    float radius = 0.1f;

    // contructors
    Ball3D();
    Ball3D(Entity3D* parent): Entity3D(parent)
    {
        material.color = QVector3D(Random::range(0.2f, 1.0f),
                          Random::range(0.2f, 1.0f) ,
                          Random::range(0.2f, 0.5f));
    };

private:
    void draw();

};

#endif // BALL3D_H
