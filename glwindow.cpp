#include "glwindow.h"

GlWindow::GlWindow(QWidget *parent):
    QGLWidget(parent)
{

}


void GlWindow::initializeGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width() - 50,height() - 50,0,0,1);
    glClearColor(0,0,0,1);
}

void GlWindow::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    makeCurrent();
    swapBuffers();
}

void GlWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,width() - 50,height() - 50,0,0,1);
    glViewport(0,0, w,h);
}

void GlWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    glBegin(GL_LINES);
    glVertex2i(20,20);
    glVertex2i(200,200);
    glEnd();
}
