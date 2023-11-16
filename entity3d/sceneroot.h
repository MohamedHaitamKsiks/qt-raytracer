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

    inline const QMatrix4x4& getCameraTransform() const
    {
        return this->cameraTransform;
    }

    inline float getCameraFOV() const
    {
        return this->cameraFOV;
    }

private:
    // sky box
    QVector3D skyColor{0.5f, 0.8f, 1.0f};
    QVector3D horizonColor{1.0, 1.0, 1.0};
    QVector3D groundColor{0.2, 0.2, 0.4};

    // ray tracer config
    int rayMaxBounce = 10;
    int samplerPerPixel = 1;

    // camera settings
    QVector3D cameraPosition{0.0f, 0.0f, 0.0f};
    QVector3D cameraRotation{0.0f, 0.0f, 0.0f};
    QMatrix4x4 cameraTransform;
    float cameraFOV = 75.0f;


    void draw();

};

#endif // SCENEROOT_H
