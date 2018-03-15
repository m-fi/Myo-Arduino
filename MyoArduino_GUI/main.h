#ifndef MAIN_H
#define MAIN_H
#include "openglwindow.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QTimer>

//header style declaration
class ModernTriangle : public OpenGLWindow
{
public:
ModernTriangle();

void initialize() Q_DECL_OVERRIDE;
void render() Q_DECL_OVERRIDE;

private:
GLuint vertexLocation;
GLuint colorLocation;
GLuint matrixLocation;

QOpenGLVertexArrayObject m_vao; // Our Vertex Array Object
QOpenGLBuffer m_vvbo; // Our vertice Vertex Buffer Object
QOpenGLBuffer m_vcbo; // Our color Vertex Buffer Object

QOpenGLShaderProgram* m_program;
};

ModernTriangle::ModernTriangle()
{
    //this calls for the window to be refreshed on a timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    timer->start(100);
}
#endif // MAIN_H
