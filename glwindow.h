#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>
#include <GL/gl.h>

class GlWindow : public QGLWidget
{
    Q_OBJECT
public:
    explicit GlWindow(QWidget *parent);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void clear();

private:

};

#endif // GLWINDOW_H
