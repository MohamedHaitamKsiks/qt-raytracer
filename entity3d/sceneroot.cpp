#include "sceneroot.h"

void SceneRoot::serializeFields(QLayout* layout)
{
    FieldSerializer::SerializeLayout("Sky Color", this->skyColor, layout);
    FieldSerializer::SerializeLayout("Horizon Color", this->horizonColor, layout);
    FieldSerializer::SerializeLayout("Ground Color", this->groundColor, layout);

    FieldSerializer::SerializeLayout("Ray Max Bounce", this->rayMaxBounce, layout);
    FieldSerializer::SerializeLayout("Samples Per Pixel", this->samplerPerPixel, layout);

    FieldSerializer::SerializeLayout("Camera Position", this->cameraPosition, layout);
    FieldSerializer::SerializeLayout("Camera Rotation", this->cameraRotation, layout);
    FieldSerializer::SerializeLayout("FOV", this->cameraFOV, layout);
}

void SceneRoot::draw()
{
    // calculate camera transform
    QMatrix4x4 rotation{};
    rotation.rotate(-QQuaternion::fromEulerAngles(this->cameraRotation));

    QMatrix4x4 translate{};
    translate.translate(this->cameraPosition);

    this->cameraTransform = translate * rotation;
}
