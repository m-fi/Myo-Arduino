#ifndef MAIN_H
#define MAIN_H

#include "openglwindow.h"
#include <QTimer>

//header style declaration
class QuatDisplay : public OpenGLWindow
{
    public:
    QuatDisplay();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

};

QuatDisplay::QuatDisplay()
{
    //this calls for the window to be refreshed on a timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(renderNow()));
    timer->start(100);
}
#endif // MAIN_H
