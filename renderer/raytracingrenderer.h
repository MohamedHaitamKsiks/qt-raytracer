#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include <QOpenGLTexture>
#include <QtOpenGL>
#include <QMatrix4x4>

#include <thread>
#include <vector>

#include <QOpenGLFunctions_4_3_Core>

#include "entity3d/entity3d.h"
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
    inline Entity3D* getSceneRoot()
    {
        return sceneRoot;
    }

    // reset renderer
    inline void resetRenderer()
    {
        frameCounter = 1;
    }


private:
    static RaytracingRenderer* s_Instance;

    // scene root
    Entity3D* sceneRoot = nullptr;

    // ray tracer config
    int rayMaxBounce = 5;
    int samplerPerPixel = 2;

    // cache uniform locations
    int frameCounterLocation;
    int sphereCommandCountLocation;
    int samplePerPixelLocation;
    int rayMaxBounceLocation;

    //gl functions
    QOpenGLFunctions_4_3_Core glFunctions{};

    // compute shader program
    QOpenGLShaderProgram* computeProgram = nullptr;

    // draw command list
    QVector<SphereCommand> sphereCommands{};

    // sphere command buffer
    uint32_t sphereCommandsBufferObject;

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

    // create all  command buffers
    void createCommandBuffers();

    // destroy all command buffers
    void destroyCommandBuffers();
};

#endif // RAYTRACINGRENDERER_H
