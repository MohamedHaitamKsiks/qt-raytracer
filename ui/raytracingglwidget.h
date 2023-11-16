#ifndef RAYTRACINGGLWIDGET_H
#define RAYTRACINGGLWIDGET_H

#include "renderer/raytracingrenderer.h"

#include <QtOpenGL>
#include <QtOpenGLWidgets>


class RaytracingGLWidget: public QOpenGLWidget
{
public:
    RaytracingGLWidget(QWidget* parent);
    ~RaytracingGLWidget();

    // init
    void initializeGL();

    // draw all
    void paintGL();

    // void resize canvas
    void resizeGL(int w, int h);


private:
    // render timer
    QTimer timer;

    // renderer
    RaytracingRenderer* renderer = nullptr;

    // renderer state
    bool isRendering = false;
    std::thread* renderThread = nullptr;

    // view port
    QRect viewport;
    int width = 0;
    int height = 0;

    // main shader to draw the canvas texture
    QOpenGLShaderProgram* mainProgram = nullptr;

    // quad vertices
    float quadVerticies[24] = {
        // position         uv
       -1.0f,  -1.0f,     0.0f, 0.0f,
        1.0f,  -1.0f,     1.0f, 0.0f,
        1.0f,   1.0f,     1.0f, 1.0f,
        1.0f,   1.0f,     1.0f, 1.0f,
       -1.0f,   1.0f,     0.0f, 1.0f,
       -1.0f,  -1.0f,     0.0f, 0.0f
    };

    // vertex buffer object for quad
    QOpenGLBuffer* vbo = nullptr;

    // craete vbo
    void createVbo();

    // create shader program
    void createShaderProgram();


};

#endif // RAYTRACINGGLWIDGET_H
