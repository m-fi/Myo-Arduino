/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>

#include "include/AntTweakBar.h"
//#include "include/glut.h"
#include <math.h>
#include <QQuaternion>

#include "main.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QScreen>
#include <QtCore/QtMath>
#include <QDebug>
float quat1[4] = {0,0,0,1};
float quat2[4] = {0,0,0,1};
float quatd[4] = {0,0,0,1};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.move(w.x(), w.y()-100);
    //new
    QSurfaceFormat format;
    format.setDepthBufferSize( 4 );
    format.setSamples(4);
    format.setVersion(3, 2);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setProfile( QSurfaceFormat::CoreProfile );

    ModernTriangle window;
    window.setFormat(format);
    int _width = 655;int _height = 225;
    window.resize(_width, _height);
    TwWindowSize(_width, _height);
    window.setTitle("Quaternion Animations");
    window.setMaximumSize(window.size());
    window.setMinimumSize(window.size());
    window.show();
    window.setPosition(window.x(),window.y()+170);
    //set focus back to the main window
    w.activateWindow();
    w.raise();

    //end new
    return a.exec();
}

void ModernTriangle::initialize()
{
    //bg colour of window
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

m_program = new QOpenGLShaderProgram();
m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
"#version 150\n" //GLSL version 1.5
"in vec3 position;\n" //attribute named position with 3 elements per vertex in
"in vec3 color;\n"
"out vec4 fragColor;\n"
"void main() {\n"
" fragColor = vec4(fragColor, 1.0);\n"
" gl_Position = vec4(position, 1.0);\n"
"}\n"
);
m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
"#version 150\n" //GLSL version 1.5
"in vec3 fragColor;\n"
"out vec4 finalcolor;\n"
"void main() {\n"
" finalcolor = vec4(fragColor, 1.0);\n"
"}\n"
);

m_program->link();
m_program->bind(); // bind Shader (Do not release until VAO is created)

//initialise AntTweakBar settings
TwInit(TW_OPENGL_CORE, NULL);
TwBar * Quat1 = TwNewBar("Quat 1 data");
TwSetParam(Quat1, NULL, "position", TW_PARAM_CSTRING, 1, "0 10");
TwSetParam(Quat1, NULL, "size", TW_PARAM_CSTRING, 1, "200 200");
TwSetParam(Quat1, NULL, "color", TW_PARAM_CSTRING, 1, "96 216 224");
//quat1
TwAddVarRO(Quat1, "Quaternion 1", TW_TYPE_QUAT4F, &quat1, "opened=true");
TwAddVarRO(Quat1, "q1 X: ", TW_TYPE_FLOAT, &quat1[0], "");
TwAddVarRO(Quat1, "q1 Y: ", TW_TYPE_FLOAT, &quat1[1], "");
TwAddVarRO(Quat1, "q1 Z: ", TW_TYPE_FLOAT, &quat1[2], "");
TwAddVarRO(Quat1, "q1 W: ", TW_TYPE_FLOAT, &quat1[3], "");
//quat2
TwBar * Quat2 = TwNewBar("Quat 2 data");
TwSetParam(Quat2, NULL, "position", TW_PARAM_CSTRING, 1, "220 10");
TwSetParam(Quat2, NULL, "size", TW_PARAM_CSTRING, 1, "200 200");
TwSetParam(Quat2, NULL, "color", TW_PARAM_CSTRING, 1, "96 216 224");
TwAddVarRO(Quat2, "Quaternion 2", TW_TYPE_QUAT4F, &quat2, "opened=true");
TwAddVarRO(Quat2, "q2 X: ", TW_TYPE_FLOAT, &quat2[0], "");
TwAddVarRO(Quat2, "q2 Y: ", TW_TYPE_FLOAT, &quat2[1], "");
TwAddVarRO(Quat2, "q2 Z: ", TW_TYPE_FLOAT, &quat2[2], "");
TwAddVarRO(Quat2, "q2 W: ", TW_TYPE_FLOAT, &quat2[3], "");
//quat difference
TwBar * Quatd = TwNewBar("Quat diff data");
TwSetParam(Quatd, NULL, "position", TW_PARAM_CSTRING, 1, "440 10");
TwSetParam(Quatd, NULL, "size", TW_PARAM_CSTRING, 1, "200 200");
TwSetParam(Quatd, NULL, "color", TW_PARAM_CSTRING, 1, "96 216 224");
TwAddVarRO(Quatd, "Quaternion difference", TW_TYPE_QUAT4F, &quatd, "opened=true");
TwAddVarRO(Quatd, "qd X: ", TW_TYPE_FLOAT, &quatd[0], "");
TwAddVarRO(Quatd, "qd Y: ", TW_TYPE_FLOAT, &quatd[1], "");
TwAddVarRO(Quatd, "qd Z: ", TW_TYPE_FLOAT, &quatd[2], "");
TwAddVarRO(Quatd, "qd W: ", TW_TYPE_FLOAT, &quatd[3], "");
//global bar styling
TwDefine( "GLOBAL fontstyle=fixed  contained=true "); //fixed-width font, bars contained to window
}

void setQuat(QQuaternion &quat1_,QQuaternion &quat2_,QQuaternion &quatD_)
{
    //update quat1
    quat1[0] = quat1_.x();
    quat1[1] = quat1_.y();
    quat1[2] = quat1_.z();
    quat1[3] = quat1_.scalar();
    //quat2
    quat2[0] = quat2_.x();
    quat2[1] = quat2_.y();
    quat2[2] = quat2_.z();
    quat2[3] = quat2_.scalar();
    //quatdifference
    quatd[0] = quatD_.x();
    quatd[1] = quatD_.y();
    quatd[2] = quatD_.z();
    quatd[3] = quatD_.scalar();
}
void ModernTriangle::render()
{
const qreal retinaScale = devicePixelRatio();
glViewport(0, 0, width() * retinaScale, height() * retinaScale);

// Clear
glClear(GL_COLOR_BUFFER_BIT);

// Render using our shader
m_program->bind();
m_vao.bind(); //sets
glDrawArrays(GL_TRIANGLES, 0, 3);

    setQuat(q1,q2,qd);
    TwDraw();
//    if(bQuat == false)
//    {
//        quat[0]=1;
//        quat[1]=0.20;
//        quat[2]=0.60;
//        quat[3]=0;
//        bQuat = true;
//    } else if(bQuat == true)
//    {
//        quat[0]=0;
//        quat[1]=1;
//        quat[2]=0;
//        quat[3]=1;
//        bQuat = false;
//    }

m_vao.release();
m_program->release();

}
//    TwInit(TW_OPENGL_CORE, NULL);
//    float quat[4] = {0,0,0,1};
//    TwBar * Quat1 = TwNewBar("Quaternion settings");
//    TwSetParam(Quat1, NULL, "position", TW_PARAM_CSTRING, 1, "808 16");
//    //TwAddVarRO(Quat1, "Quaternion", TW_TYPE_QUAT4F, &gOrientation2, "showval=true open=true ");
//    TwAddVarRO(Quat1, "Quaternion", TW_TYPE_QUAT4F, &quat, "");

//    TwDraw();
