#include "raytracingrenderer.h"
#include <QFile>
#include <QMessageBox>

#define VERTEX_SHADER_PATH ":/shaders/default.vert"
#define FRAGMENT_SHADER_PATH ":/shaders/default.frag"

#define ASPECT_RATIO 1.77f

#define V_POSITION 0
#define V_TEXTURE_COORD 1

RaytracingRenderer::RaytracingRenderer(QWidget* parent):
    QOpenGLWidget(parent)
{
    this->setMinimumSize(QSize(320, 180));
}



RaytracingRenderer::~RaytracingRenderer()
{
    // destroy texture
    this->canvasTexture->destroy();
    delete this->canvasTexture;

    // destroy vbo
    this->vbo->destroy();
    delete vbo;

    // destroy program
    delete this->mainProgram;
}

void RaytracingRenderer::initializeGL()
{
    // create shader program
    this->createShaderProgram();

    // create vbo
    this->createVbo();


    // init update
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(0);
}

void RaytracingRenderer::createVbo()
{
    this->vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo->bind();
    vbo->allocate(this->quadVerticies, sizeof(this->quadVerticies));
}

void RaytracingRenderer::createShaderProgram()
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

    if (!this->mainProgram->link())
    {
        QMessageBox::information(0, "error", this->mainProgram->log());
        return;
    }
}

void RaytracingRenderer::createCanvasTexture(int width, int height)
{
    if (!this->canvasTexture)
        this->canvasTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);

    // destroy old texture
    if (this->canvasTexture->isCreated())
        this->canvasTexture->destroy();

    // generate red texture
    QImage image(width, height, QImage::Format_RGBA64);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            QPoint point(i, j);
            QColor color(255, 0, 0, 255);

            image.setPixelColor(point, color);
        }
    }

    this->canvasTexture->create();
    this->canvasTexture->bind();
    this->canvasTexture->setData(image);

}


float Time = 0.0f;

void RaytracingRenderer::paintGL()
{
    auto* glFunctions = this->context()->functions();

    glFunctions->glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
    glFunctions->glClear(GL_COLOR_BUFFER_BIT);

    this->mainProgram->bind();

    // set texture
    glFunctions->glActiveTexture(GL_TEXTURE0);
    this->canvasTexture->bind();

    int textureLocation = this->mainProgram->uniformLocation("u_Texture");
    this->mainProgram->setUniformValue(textureLocation, 0);

    // set time
    int timeLocation = this->mainProgram->uniformLocation("u_Time");
    Time += 0.1f;
    this->mainProgram->setUniformValue(timeLocation, Time);

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

void RaytracingRenderer::resizeGL(int w, int h)
{
    auto* glFunctions = context()->functions();
    glFunctions->glViewport(0, 0, w, h);
    createCanvasTexture(w, h);
}
