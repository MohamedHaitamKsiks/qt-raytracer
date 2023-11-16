#include "meshinstance3d.h"
#include "renderer/raytracingrenderer.h"

#define MESH_PASH_PREFIX ":/models/"

void MeshInstance3D::serializeFields(QLayout* layout)
{
    Entity3D::serializeFields(layout);
    FieldSerializer::SerializeLayout("Mesh Path", this->meshPath, layout);
}

void MeshInstance3D::draw()
{
    auto* renderer = RaytracingRenderer::instance();

    // check if model is loaded skip drawing if not valid
    QString fullMeshPath = MESH_PASH_PREFIX + this->meshPath;
    if (!renderer->getMeshIndices().contains(fullMeshPath) && !renderer->loadMesh(fullMeshPath))
        return;

    // draw mesh
    MeshInstanceCommand command;
    command.meshIndex = renderer->getMeshIndex(fullMeshPath);
    this->globalTransfrom.copyDataTo(command.transform);
    command.material = this->material;

    renderer->drawMesh(command);
}
