#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include "material.h"

// add this to the end of a gl struct to aligned with 16 bytes
#define GL_STRUCT __attribute__ ((aligned(16)));

// sphrere command
struct SphereCommand
{
    QVector3D center{0.0f, 0.0f, 0.0f};
    float radius = 0.0f;
    Material material{};
} GL_STRUCT;

#endif // DRAWCOMMAND_H
