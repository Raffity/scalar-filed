#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>
#include <GL/gl.h>
#include <marchingcubes.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


class GlWindow : public QGLWidget
{
    Q_OBJECT
public:
    bool flatten = false;
    double multiplier = 1;
    int invert = -1;
    float stepZ = 0.2, stepX = 0.2, stepY = 0.2;
    float transZ = -8, transX = 0, transY = 0;
    float angleX = 0, angleY = 0, angleZ = 0;
    float saX = 2, saY = 2, saZ = 2;
    float MINX = -10, MAXX = 10, MINY = -10, MAXY = 10, MINZ = -10, MAXZ = 10;

    float isoVlaue = 1;
    int nX, nY, nZ;
    mp4Vector * mcPoints;
    TRIANGLE * Triangles;
    int numOfTriangles;


    explicit GlWindow(QWidget *parent);
    void initializeGL();
    void paintGL();
    void updateGL(bool needReculculate = false);
    void mouseMoveEvent(QMouseEvent *pe);
    void wheelEvent(QWheelEvent *pe);
    void keyPressEvent(QKeyEvent *pe);
    void resizeGL(int w, int h);
    void clear();

    void RunMarchingCubes();
    void InitData();
    void culculate();
    float density_function(MpVector p);

private:

};

#endif // GLWINDOW_H
