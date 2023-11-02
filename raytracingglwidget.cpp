#include "raytracingglwidget.h"
#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <QErrorMessage>


#define VERTEX_SHADER_PATH ":/shaders/default.vert"
#define FRAGMENT_SHADER_PATH ":/shaders/default.frag"

#define ASPECT_RATIO 1.77f

#define V_POSITION 0
#define V_TEXTURE_COORD 1

RaytracingGLWidget::RaytracingGLWidget(QWidget* parent):
    QOpenGLWidget(parent)
{
    this->setMinimumSize(QSize(320, 180));
}



RaytracingGLWidget::~RaytracingGLWidget()
{
    // destroy renderer
    delete this->renderer;

    // destroy vbo
    this->vbo->destroy();
    delete vbo;

    // destroy program
    delete this->mainProgram;
}

void RaytracingGLWidget::initializeGL()
{
    auto* glFunctions = this->context()->functions();

    // print gl info
    const char *renderHardware = (char*) glFunctions->glGetString(GL_RENDERER);
    qInfo() << "Using Hardware:" <<  QString(renderHardware);
    qInfo() << "Widget OpenGl: " << format().majorVersion() << "." << format().minorVersion();
    qInfo() << "Context valid: " << context()->isValid();
    qInfo() << "Really used OpenGl: " << context()->format().majorVersion() << "." << context()->format().minorVersion();

    // set clear color
    glFunctions->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // create shader program
    this->createShaderProgram();

    // create vbo
    this->createVbo();

    // create raytracing rendere
    this->renderer = new RaytracingRenderer();

    // init update
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(0);
}

void RaytracingGLWidget::createVbo()
{
    this->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo->bind();
    vbo->allocate(this->quadVerticies, sizeof(this->quadVerticies));
}

void RaytracingGLWidget::createShaderProgram()
{
    this->mainProgram = new QOpenGLShaderProgram();

    // load vertex
    if (!this->mainProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, VERTEX_SHADER_PATH))
    {
        QMessageBox::information(0, "error", this->mainProgram->log());
        return;
    }

    // add fragment
    if (!this->mainProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, FRAGMENT_SHADER_PATH))
    {
        QMessageBox::information(0, "error", this->mainProgram->log());
        return;
    }

    // link program
    if (!this->mainProgram->link())
    {
        QMessageBox::information(0, "error", this->mainProgram->log());
        return;
    }

    // set sampler for u_Texture to be 0 by default won't change later since this is all the rendering we'll be doing
    auto* glFunctions = this->context()->functions();
    glFunctions->glActiveTexture(GL_TEXTURE0);

    int textureLocation = this->mainProgram->uniformLocation("u_Texture");
    this->mainProgram->setUniformValue(textureLocation, 0);
}


void RaytracingGLWidget::paintGL()
{
    auto* glFunctions = this->context()->functions();

    glFunctions->glViewport(viewport.x(), viewport.y(), viewport.width(), viewport.height());
    glFunctions->glClear(GL_COLOR_BUFFER_BIT);

    // rendere scene to texture
    this->renderer->draw(this->context(), viewport.width(), viewport.height());

    // draw texture
    this->mainProgram->bind();

    // pass frame number
    int frameCounterLocation = this->mainProgram->uniformLocation("u_FrameCounter");
    this->mainProgram->setUniformValue(frameCounterLocation, this->renderer->getFrameCounter());

    //bind vertex attributes
    // get position attribute
    glFunctions->glVertexAttribPointer(V_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void *) (0));
    this->mainProgram->enableAttributeArray(V_POSITION);

    // get texture coordinates attribute
    glFunctions->glVertexAttribPointer(V_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void *) (sizeof(float) * 2));
    this->mainProgram->enableAttributeArray(V_TEXTURE_COORD);

    // draw quad
    this->vbo->bind();
    glFunctions->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RaytracingGLWidget::resizeGL(int w, int h)
{
    float width = w;
    float height = h;

    // get aspect
    float aspectRatio = width / height;

    // same aspect ratio
    float precision = 0.0001f;
    if (abs(aspectRatio - ASPECT_RATIO) < precision)
    {
        // recompute viewport
        viewport.setRect(0, 0, w, h);
    }
    // window larger than viewport
    else if (aspectRatio > ASPECT_RATIO)
    {
        // get new width
        int newWidth = (int) (ASPECT_RATIO * height);
        int viewportPosition = (w - newWidth) / 2;
        viewport.setRect(viewportPosition, 0, newWidth, h);
    }
    // window longuer than viewport
    else
    {
        // get new height
        int newHeight = (int)(width / ASPECT_RATIO);
        int viewportPosition = (h - newHeight) / 2;
        viewport.setRect(0, viewportPosition, w, newHeight);
    }
}
