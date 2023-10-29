#ifndef RAYTRACINGRENDERER_H
#define RAYTRACINGRENDERER_H

#include <QtOpenGL>
#include <QtOpenGLWidgets>


class RaytracingRenderer: public QOpenGLWidget
{
public:
    RaytracingRenderer(QWidget* parent);
    ~RaytracingRenderer();


    // init
    void initializeGL();

    // draw all
    void paintGL();

    // void resize canvas
    void resizeGL(int w, int h);


private:

    QTimer timer;

    // main shader to draw the canvas texture
    QOpenGLShaderProgram* mainProgram = nullptr;

    // texture to where you render
    QOpenGLTexture* canvasTexture = nullptr;
    int width = 0;
    int height = 0;

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

    // create canvas texture
    void createCanvasTexture(int width, int height);


};

#endif // RAYTRACINGRENDERER_H
