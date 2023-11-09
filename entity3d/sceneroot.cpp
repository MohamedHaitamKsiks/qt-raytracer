#include "sceneroot.h"

void SceneRoot::serializeFields(QLayout* layout)
{
    FieldSerializer::SerializeLayout("Sky Color", this->skyColor, layout);
    FieldSerializer::SerializeLayout("Horizon Color", this->horizonColor, layout);
    FieldSerializer::SerializeLayout("Ground Color", this->groundColor, layout);

    FieldSerializer::SerializeLayout("Ray Max Bounce", this->rayMaxBounce, layout);
    FieldSerializer::SerializeLayout("Samples Per Pixel", this->samplerPerPixel, layout);
}
