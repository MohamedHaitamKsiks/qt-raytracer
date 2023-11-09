#include "raytracingrenderer.h"
#include "entity3d/ball3d.h"
#include "entity3d/entitymanager.h"

#include <QMessageBox>


#define SPHERE_COMMAND_MAX 16000
#define COMPUTE_SHADER_PATH ":/shaders/raytracer.glsl"

RaytracingRenderer* RaytracingRenderer::s_Instance = nullptr;

RaytracingRenderer::RaytracingRenderer()
{
    // set singleton,only one instanc is allowed
    assert(s_Instance == nullptr);
    s_Instance = this;

    // create scene
    this->sceneRoot = new SceneRoot();
    this->sceneRoot->localPosition = QVector3D(0.0f, 0.0f, 0.0f);

}

void RaytracingRenderer::init()
{
    // get opengl 4.3 functions
    this->glFunctions.initializeOpenGLFunctions();

    // create program
    this->computeProgram = new QOpenGLShaderProgram();
    if (!this->computeProgram->addShaderFromSourceFile(QOpenGLShader::Compute, COMPUTE_SHADER_PATH))
        QMessageBox::information(0, "error", this->computeProgram->log());
    this->computeProgram->link();

    // cache uniform locations
    this->frameCounterLocation = this->computeProgram->uniformLocation("u_FrameCounter");
    this->sphereCommandCountLocation = this->computeProgram->uniformLocation("u_SphereCommandCount");
    this->samplePerPixelLocation = this->computeProgram->uniformLocation("u_SamplePerPixel");
    this->rayMaxBounceLocation =this->computeProgram->uniformLocation("u_RayMaxBounce");
    this->skyColorLocation = this->computeProgram->uniformLocation("u_SkyColor");
    this->horizonColorLocation = this->computeProgram->uniformLocation("u_HorizonColor");
    this->groundColorLocation = this->computeProgram->uniformLocation("u_GroundColor");

    // create data on gpu
    this->createCanvasTexture();
    this->createCommandBuffers();
}

RaytracingRenderer::~RaytracingRenderer()
{
    s_Instance = nullptr;

    delete this->computeProgram;
    this->destroyCanvasTexture();
    this->destroyCommandBuffers();

    delete this->sceneRoot;
}

void RaytracingRenderer::draw(QOpenGLContext* context, int width, int height)
{
    // increment frame counter
    this->frameCounter++;

    // update scene
    this->sceneRoot->update();

    // generate red texture
    if (this->canvasWidth != width || this->canvasHeight != height)
    {
        // reset frame counter
        this->frameCounter = 1;

        // set canvas size
        this->canvasWidth = width;
        this->canvasHeight = height;

        // recreate texture
        this->destroyCanvasTexture();
        this->createCanvasTexture();
    }

    // use compute shader
    this->computeProgram->bind();

    // push draw commands to the shader
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->sphereCommands.size() * sizeof(SphereCommand), this->sphereCommands.data());

    // send unigform values
    this->computeProgram->setUniformValue(this->frameCounterLocation, this->frameCounter);
    this->computeProgram->setUniformValue(this->sphereCommandCountLocation, static_cast<int>(this->sphereCommands.size()));

    this->computeProgram->setUniformValue(this->rayMaxBounceLocation,this->sceneRoot->getRayMaxBounce());
    this->computeProgram->setUniformValue(this->samplePerPixelLocation, this->sceneRoot->getSamplerPerPixel());

    this->computeProgram->setUniformValue(this->skyColorLocation,this->sceneRoot->getSkyColor());
    this->computeProgram->setUniformValue(this->horizonColorLocation, this->sceneRoot->getHorizonColor());
    this->computeProgram->setUniformValue(this->groundColorLocation,this->sceneRoot->getGroundColor());

    // start compute
    this->glFunctions.glDispatchCompute(width, height, 1);

    // wait for compute shader to finish
    this->glFunctions.glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // clear commands
    this->sphereCommands.clear();
}

void RaytracingRenderer::createCanvasTexture()
{
    this->canvasTexture.setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Nearest);
    this->canvasTexture.create();
    this->canvasTexture.bind();

    this->glFunctions.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, this->canvasWidth, this->canvasHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    this->glFunctions.glBindImageTexture(0, this->canvasTexture.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void RaytracingRenderer::destroyCanvasTexture()
{
    if (this->canvasTexture.isCreated())
        this->canvasTexture.destroy();
}


void RaytracingRenderer::createCommandBuffers()
{

    this->glFunctions.glGenBuffers(1, &this->sphereCommandsBufferObject);
    this->glFunctions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->sphereCommandsBufferObject); // bind buffer
    this->glFunctions.glBufferData(GL_SHADER_STORAGE_BUFFER, SPHERE_COMMAND_MAX * sizeof(SphereCommand), nullptr, GL_DYNAMIC_DRAW); // reserve storage place
    this->glFunctions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->sphereCommandsBufferObject);

}


void RaytracingRenderer::destroyCommandBuffers()
{
    this->glFunctions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // ubind buffer
    this->glFunctions.glDeleteBuffers(1, &this->sphereCommandsBufferObject); // delete buffer
}
