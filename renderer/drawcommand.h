#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include <QMatrix4x4>
#include "material.h"

// add this to the end of a gl struct to aligned with 16 bytes
#define GL_STRUCT __attribute__ ((aligned(16)));

// sphrere command
struct SphereCommand
{
    QVector3D center{0.0f, 0.0f, 0.0f};
    float radius = 0.0f;
    Material material{};
} GL_STRUCT;

// vertex
struct Vertex
{
    QVector3D position{};
    QVector3D normal{};
} GL_STRUCT;

// mesh 3d info
struct MeshInfo
{
    int startIndex = 0;
    int vertexCount = 0;
} GL_STRUCT;

// mesh instance
struct MeshInstanceCommand
{
    int meshIndex = 0;
    float transform[16];
    float PADDING2[3];
    Material material{};
} GL_STRUCT;

#endif // DRAWCOMMAND_H
