#include "meshinstance3d.h"
#include "renderer/raytracingrenderer.h"

void MeshInstance3D::serializeFields(QLayout* layout)
{
    Entity3D::serializeFields(layout);
}

void MeshInstance3D::draw()
{
    auto* renderer = RaytracingRenderer::instance();

    MeshInstanceCommand command;
    command.meshIndex = renderer->getMeshIndex(this->meshPath);
    //command.transform = this->getGlobalTransform();
    command.material = material;

    renderer->drawMesh(command);
}
