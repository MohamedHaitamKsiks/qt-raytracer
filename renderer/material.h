#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

struct Material
{
    QVector3D color{1.0f, 1.0f, 1.0f};
    float smoothness = 0.0f;
    int emissive = 0;
};

#endif // MATERIAL_H
