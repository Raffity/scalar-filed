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
    explicit GlWindow(QWidget *parent);
    void initializeGL();
    void paintGL();
    void mouseMoveEvent(QMouseEvent *pe);
    void wheelEvent(QWheelEvent *pe);
    void keyPressEvent(QKeyEvent *pe);
    void resizeGL(int w, int h);
    void clear();
    void RunMarchingCubesTest();
    void InitData();

    float stepZ, stepX, stepY;
    float transZ, transX, transY;
    float angleX, angleY, angleZ;
    float saX, saY, saZ;

    float minValue;
    int nX, nY, nZ;
    mp4Vector * mcPoints;
    TRIANGLE * Triangles;
    int numOfTriangles;

private:

};

#define MINX -20.0
#define MAXX 20.0
#define MINY -20.0
#define MAXY 20.0
#define MINZ -20.0
#define MAXZ 20.0

float Potential(MpVector p);

void drawCoordinates(float s);


#endif // GLWINDOW_H
