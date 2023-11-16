#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector4D>

struct Material
{
    QVector3D color{0.2f, 0.2f, 0.2f};
    float smoothness = 0.0f;
    int emissive = 0;
    float refraction = 1.0f;
    float transparency = 0.0f;
};


#endif // MATERIAL_H
