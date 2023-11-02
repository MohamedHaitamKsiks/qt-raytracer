#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "material.h"

// sphrere command
struct SphereCommand
{
    QVector3D center{0.0f, 0.0f, 0.0f};
    float radius = 0.0f;
    Material material;
    float PADDING[3];
};

#endif // DRAWCOMMAND_H
