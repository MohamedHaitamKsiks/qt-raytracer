#include "cube3d.h"
#include "renderer/raytracingrenderer.h"

#define CUBE_3D "Cube3D"

void Cube3D::loadCubeMesh()
{
    QVector<Vertex> vertices = {
        Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},

        Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

        Vertex{{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},

        Vertex{{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        Vertex{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        Vertex{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},

        Vertex{{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},
        Vertex{{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},
        Vertex{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}},

        Vertex{{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        Vertex{{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        Vertex{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}
    };


    QVector<int> indices = {
        0, 1, 3,
        3, 1, 2,

        4, 5, 7,
        7, 5, 6,

        8, 9, 11,
        11, 9, 10,

        12, 13, 15,
        15, 13, 14,

        16, 17, 19,
        19, 17, 18,

        20, 21, 23,
        23, 21, 22
    };

    // register mesh
    auto* renderer = RaytracingRenderer::instance();
    renderer->loadMesh(CUBE_3D, vertices, indices);

}


void Cube3D::draw()
{
    // get renderer
    auto* renderer = RaytracingRenderer::instance();

    // check if cube has been loaded
    if (!renderer->getMeshIndices().contains(CUBE_3D))
        this->loadCubeMesh();

    // draw cube
    MeshInstanceCommand command;
    command.meshIndex = renderer->getMeshIndex(CUBE_3D);
    this->globalTransfrom.copyDataTo(command.transform);
    command.material = this->material;

    renderer->drawMesh(command);
}

void Cube3D::serializeFields(QLayout* layout)
{
    Entity3D::serializeFields(layout);
}
