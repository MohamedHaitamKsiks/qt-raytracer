#include "ball3d.h"
#include "renderer/raytracingrenderer.h"

Ball3D::Ball3D()
{

}


void Ball3D::draw()
{
    SphereCommand command;
    command.material = this->material;
    command.center = (this->getGlobalTransform() * QVector4D(0.0f, 0.0f, 0.0f, 1.0f)).toVector3D();
    command.radius = this->radius;

    RaytracingRenderer::instance()->drawSphere(command);
}
