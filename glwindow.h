#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>
#include <GL/gl.h>
#include <marchingcubes.h>

class GlWindow : public QGLWidget
{
    Q_OBJECT
public:
    explicit GlWindow(QWidget *parent);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void clear();
    void RunMarchingCubesTest();
    void InitData();

    float stepZ, stepX, stepY;		//increment step for moving camera
    float transZ, transX, transY;	//translates camera by this amount on each axis
    float angleX, angleY, angleZ;	//angles of rotation
    float saX, saY, saZ;			//increments on angles

    //minimal value and number of cells on each axis: passed into Marching Cubes
    float minValue;
    int nX, nY, nZ;
    //data points passed to Marching Cubes
    mp4Vector * mcPoints;
    //data returned by Marching Cubes
    TRIANGLE * Triangles;
    int numOfTriangles;

private:

};

//boundary values for Marching Cubes
#define MINX -4.0
#define MAXX 5.0
#define MINY -8.0
#define MAXY 8.0
#define MINZ -8.0
#define MAXZ 8.0

//function which computes a value at point p
float Potential(MpVector p);

//draws coordinates of length s on all axis
void drawCoordinates(float s);


#endif // GLWINDOW_H
