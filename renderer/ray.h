#ifndef RAY_H
#define RAY_H

#include <QVector3D>
#include <QColor>
#include "material.h"

// ray definition
struct Ray
{
    QVector3D origin;
    QVector3D direction;
    QVector4D color{1.0f, 1.0f, 1.0f, 1.0f};

    inline QVector3D at(float t) const
    {
        return origin + direction * t;
    }
};

// ray hit info
struct RayHitInfo
{
    QVector3D position;
    QVector3D normal;
    float depth = 10000000.0f;
    bool hit = false;
    // material info
    Material material;
};


#endif // RAY_H
