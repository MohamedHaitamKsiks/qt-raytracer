#ifndef SCENEROOT_H
#define SCENEROOT_H

#include "entity3d.h"

class SceneRoot : public Entity3D
{
public:
    SceneRoot();

    void serializeFields(QLayout* layout);
private:
    // sky box
    QVector3D skyColor;
    QVector3D HorizonColor;
    QVector3D groundColor;

    //

};

#endif // SCENEROOT_H
