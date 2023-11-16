#ifndef SCENEROOT_H
#define SCENEROOT_H

#include "entity3d.h"

class SceneRoot : public Entity3D
{
public:
    void serializeFields(QLayout* layout);

    // get sky params
    inline const QVector3D& getSkyColor() const
    {
        return this->skyColor;
    }
    inline const QVector3D& getHorizonColor() const
    {
        return this->horizonColor;
    }
    inline const QVector3D& getGroundColor() const
    {
        return this->groundColor;
    }

    // get renderer params
    inline int getRayMaxBounce() const
    {
        return this->rayMaxBounce;
    }
    inline int getSamplerPerPixel() const
    {
        return this->samplerPerPixel;
    }


private:
    // sky box
    QVector3D skyColor{0.5f, 0.8f, 1.0f};
    QVector3D horizonColor{1.0, 1.0, 1.0};
    QVector3D groundColor{0.2, 0.2, 0.4};

    // ray tracer config
    int rayMaxBounce = 2;
    int samplerPerPixel = 1;


};

#endif // SCENEROOT_H
