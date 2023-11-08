#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

struct Material
{
    QVector3D color{0.2f, 0.2f, 0.2f};
    float smoothness = 1.0f;
    int emissive = 0;
};


#endif // MATERIAL_H
