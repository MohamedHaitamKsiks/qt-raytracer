#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include <QOpenGLTexture>
#include <QtOpenGL>
#include <QMatrix4x4>

#include <thread>
#include <vector>

#include <QOpenGLFunctions_4_3_Core>
#include <QHash>

#include "entity3d/entity3d.h"
#include "entity3d/sceneroot.h"
#include "drawcommand.h"

// raytracing renderer singleton
class RaytracingRenderer
{
public:
    RaytracingRenderer();
    ~RaytracingRenderer();

    // init renderer
    void init();

    // get singleton instance
    static inline RaytracingRenderer* instance()
    {
        return s_Instance;
    }

    // draw shere
    inline void drawSphere(const SphereCommand& command)
    {
        this->sphereCommands.append(command);
    }

    // draw content and return the texture
    void draw(QOpenGLContext* context, int width, int height);

    // get frame count
    inline int getFrameCounter() const
    {
        return frameCounter;
    }

    // get scene root
    inline SceneRoot* getSceneRoot()
    {
        return sceneRoot;
    }

    // reset renderer
    inline void resetRenderer()
    {
        this->destroyCanvasTexture();
        this->createCanvasTexture();
        this->frameCounter = 1;
    }

    // load mesh from file
    void loadMesh(const QString& meshPath);

    // load file from vertex files
    void loadMesh(const QString& meshName, const QVector<Vertex>& vertices, QVector<int> indices);

    // get mesh index
    inline int getMeshIndex(const QString& meshPath) const
    {
        return this->meshIndices[meshPath];
    }

    // draw mesh
    inline void drawMesh(const MeshInstanceCommand& command)
    {
        this->meshInstanceCommands.append(command);
    }

    // get mesh indies
    inline const QHash<QString, int>& getMeshIndices() const
    {
        return this->meshIndices;
    }


private:
    static RaytracingRenderer* s_Instance;

    // scene root
    SceneRoot* sceneRoot = nullptr;

    // mesh
    QHash<QString, int> meshIndices{};
    int meshCount = 0;
    int lastMeshIndex = 0;
    int vertexCount = 0;

    // cache uniform locations
    int frameCounterLocation;
    int sphereCommandCountLocation;
    int meshInstanceCommandCountLocation;
    int samplePerPixelLocation;
    int rayMaxBounceLocation;
    int skyColorLocation;
    int horizonColorLocation;
    int groundColorLocation;

    //gl functions
    QOpenGLFunctions_4_3_Core glFunctions{};

    // compute shader program
    QOpenGLShaderProgram* computeProgram = nullptr;

    // draw command list
    QVector<SphereCommand> sphereCommands{};
    QVector<MeshInstanceCommand> meshInstanceCommands{};

    // sphere command buffer
    uint32_t sphereCommandsBufferObject;
    uint32_t meshInstanceCommandsBufferObject;
    uint32_t meshInfosBufferObject;
    uint32_t vertexBufferObject;
    uint32_t indexBufferObject;

    // frame counter
    int frameCounter = 1;

    // canvas size
    int canvasWidth = 0;
    int canvasHeight = 0;

    // texture to where you render
    QOpenGLTexture canvasTexture{QOpenGLTexture::Target2D};

    // create texture
    void createCanvasTexture();

    // destroy texture
    void destroyCanvasTexture();

    // create shader buffer
    void createShaderBuffer(uint32_t& buffer, uint32_t binding, size_t size);

    // bind shader buffer
    void bindShaderBuffer(uint32_t buffer);

    // create all  command buffers
    void createCommandBuffers();

    // destroy all command buffers
    void destroyCommandBuffers();
};

#endif // RAYTRACINGRENDERER_H
