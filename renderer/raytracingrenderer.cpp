#include "raytracingrenderer.h"
#include "entity3d/ball3d.h"

#include <QMessageBox>


#define SPHERE_COMMAND_MAX 16000
#define COMPUTE_SHADER_PATH ":/shaders/raytracer.glsl"

RaytracingRenderer* RaytracingRenderer::s_Instance = nullptr;

RaytracingRenderer::RaytracingRenderer()
{
    // set singleton,only one instanc is allowed
    assert(s_Instance == nullptr);
    s_Instance = this;

    // get opengl 4.3 functions
    glFunctions.initializeOpenGLFunctions();

    // create program
    this->computeProgram = new QOpenGLShaderProgram();
    if (!computeProgram->addShaderFromSourceFile(QOpenGLShader::Compute, COMPUTE_SHADER_PATH))
        QMessageBox::information(0, "error", this->computeProgram->log());
    computeProgram->link();

    // create data on gpu
    this->createCanvasTexture();
    this->createCommandBuffers();

    // create scene
    sceneRoot = new Entity3D();

    // fill scene
    {
        auto* ball = new Ball3D(sceneRoot);
        ball->localPosition = QVector3D(0,-30.2,4);
        ball->radius = 30.0f;
        ball->material.smoothness = 0.0f;
        ball->material.color = QVector3D(0.333, 0.0, 1.0);

        auto* ball1 = new Ball3D(sceneRoot);
        ball1->localPosition = QVector3D(5.8f,2.8f,-6.0f);
        ball1->radius = 5.3f;
        ball1->material.smoothness = 1.0f;
        ball1->material.emissive = true;
        ball1->material.color = QVector3D(1.0f, 1.0f, 1.0f);

        auto* ball2 = new Ball3D(sceneRoot);
        ball2->localPosition = QVector3D(0.3f,0.1f,2.0f);
        ball2->radius = 0.3f;
        ball2->material.smoothness = 0.0f;
        ball2->material.color = QVector3D(1.0, 0.552, 0.0400);

        auto* ball3 = new Ball3D(sceneRoot);
        ball3->localPosition = QVector3D(-0.35f,0.1f,2.5f);
        ball3->radius = 0.3f;
        ball3->material.smoothness = 0.8f;
        ball3->material.color = QVector3D(1.0, 0.552, 0.04);

        auto* ball4 = new Ball3D(sceneRoot);
        ball4->localPosition = QVector3D(-0.8f,0.0f,1.5f);
        ball4->radius = 0.2f;
        ball4->material.smoothness = 1.0f;
        ball4->material.color = QVector3D(0.8f, 0.8f, 0.8f);
    }

}

RaytracingRenderer::~RaytracingRenderer()
{
    s_Instance = nullptr;

    delete this->computeProgram;
    this->destroyCanvasTexture();
    this->destroyCommandBuffers();

    delete sceneRoot;
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
    computeProgram->bind();

    // push draw commands to the shader
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->sphereCommands.size() * sizeof(SphereCommand), this->sphereCommands.data());

    // send frame counter
    int frameCounterLocation = computeProgram->uniformLocation("u_FrameCounter");
    computeProgram->setUniformValue(frameCounterLocation, this->frameCounter);

    // send sphere command count
    int sphereCommandCountLocation = computeProgram->uniformLocation("u_SphereCommandCount");
    computeProgram->setUniformValue(sphereCommandCountLocation,(int) this->sphereCommands.size());

    // start compute
    glFunctions.glDispatchCompute(width, height, 1);

    // wait for compute shader to finish
    glFunctions.glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

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
