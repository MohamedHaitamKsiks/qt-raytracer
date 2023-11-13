#include "raytracingrenderer.h"
#include "entity3d/ball3d.h"
#include "entity3d/entitymanager.h"

#include <QMessageBox>


#define SPHERE_COMMAND_MAX 16000
#define MESH_INSTANCE_COMMAND_MAX 16000
#define MESH_INFOS_MAX 16000
#define VERTEX_MAX 16000
#define INDEX_MAX 16000

#define COMPUTE_SHADER_PATH ":/shaders/raytracer.glsl"

#define SPHERE_COMMANDS_BUFFER_BINDING 3
#define MESH_INFOS_BUFFER_BINDING 5
#define MESH_INSTANCE_COMMANDS_BUFFER_BINDING 4
#define VERTEX_BUFFER_BINDING 6
#define INDEX_BUFFER_BINDING 7


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
    this->meshInstanceCommandCountLocation = this->computeProgram->uniformLocation("u_MeshInstanceCommandCount");
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
        // set canvas size
        this->canvasWidth = width;
        this->canvasHeight = height;

        // recreate texture
        this->resetRenderer();
    }

    // use compute shader
    this->computeProgram->bind();

    // push draw commands to the shader
    this->bindShaderBuffer(this->sphereCommandsBufferObject);
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->sphereCommands.size() * sizeof(SphereCommand), this->sphereCommands.data());

    this->bindShaderBuffer(this->meshInstanceCommandsBufferObject);
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->meshInstanceCommands.size() * sizeof(MeshInstanceCommand), this->meshInstanceCommands.data());

    // send unigform values
    this->computeProgram->setUniformValue(this->frameCounterLocation, this->frameCounter);
    this->computeProgram->setUniformValue(this->sphereCommandCountLocation, static_cast<int>(this->sphereCommands.size()));
    this->computeProgram->setUniformValue(this->meshInstanceCommandCountLocation, static_cast<int>(this->meshInstanceCommands.size()));

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
    this->meshInstanceCommands.clear();
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
    this->createShaderBuffer(this->sphereCommandsBufferObject, SPHERE_COMMANDS_BUFFER_BINDING, SPHERE_COMMAND_MAX * sizeof(SphereCommand));

    this->createShaderBuffer(this->vertexBufferObject, VERTEX_BUFFER_BINDING, VERTEX_MAX * sizeof(Vertex));
    this->createShaderBuffer(this->indexBufferObject, INDEX_BUFFER_BINDING, INDEX_MAX * sizeof(int));

    this->createShaderBuffer(this->meshInfosBufferObject, MESH_INFOS_BUFFER_BINDING, MESH_INFOS_MAX * sizeof(MeshInfo));
    this->createShaderBuffer(this->meshInstanceCommandsBufferObject, MESH_INSTANCE_COMMANDS_BUFFER_BINDING, MESH_INSTANCE_COMMAND_MAX * sizeof(MeshInstanceCommand));
}


void RaytracingRenderer::destroyCommandBuffers()
{
    this->glFunctions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // ubind buffer
    this->glFunctions.glDeleteBuffers(1, &this->sphereCommandsBufferObject); // delete buffer
    this->glFunctions.glDeleteBuffers(1, &this->meshInstanceCommandsBufferObject);
    this->glFunctions.glDeleteBuffers(1, &this->vertexBufferObject);
    this->glFunctions.glDeleteBuffers(1, &this->indexBufferObject);
    this->glFunctions.glDeleteBuffers(1, &this->meshInfosBufferObject);
}


void RaytracingRenderer::createShaderBuffer(uint32_t& buffer, uint32_t binding, size_t size)
{
    this->glFunctions.glGenBuffers(1, &buffer);
    this->glFunctions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer); // bind buffer
    this->glFunctions.glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); // reserve storage place
    this->glFunctions.glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, buffer);
}


void RaytracingRenderer::bindShaderBuffer(uint32_t buffer)
{
    this->glFunctions.glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
}

void RaytracingRenderer::loadMesh(const QString& meshPath)
{

}

void RaytracingRenderer::loadMesh(const QString& meshName, const QVector<Vertex>& vertices, QVector<int> indices)
{
    // preprocess indices
    for (int i = 0; i < indices.size(); i++)
    {
        indices[i] += vertexCount;
    }

    // create mesh info
    MeshInfo meshInfo;
    meshInfo.startIndex = lastMeshIndex;
    meshInfo.vertexCount = indices.count();

    // send mesh info
    this->bindShaderBuffer(this->meshInfosBufferObject);
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, this->meshCount * sizeof(MeshInfo), sizeof(MeshInfo), &meshInfo);

    // send vertex data
    this->bindShaderBuffer(this->vertexBufferObject);
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, this->vertexCount * sizeof(Vertex), vertices.size() * sizeof(Vertex), vertices.data());
    this->vertexCount += vertices.size();

    // send index data
    this->bindShaderBuffer(this->indexBufferObject);
    this->glFunctions.glBufferSubData(GL_SHADER_STORAGE_BUFFER, this->lastMeshIndex * sizeof(int), indices.size() * sizeof(int), indices.data());
    this->lastMeshIndex += indices.size();

    // register mesh
    this->meshIndices[meshName] = this->meshCount;
    meshCount++;
}
