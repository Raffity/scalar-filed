#include "glwindow.h"

GlWindow::GlWindow(QWidget *parent):
    QGLWidget(parent)
{

}


void GlWindow::initializeGL()
{
    stepZ = 0.2;
    stepX = 0.2;
    stepY = 0.2;
    transZ = -8;
    transX =0;
    transY = 0;
    angleX= 0;
    angleY = 0;
    angleZ = 0;
    saX = 5;
    saY = 5;
    saZ = 5;
    minValue = 10;
    nX = 100;
    nY = 100;
    nZ = 100;
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float light_color[] = {1, 1, 1, 1};
    float light_position[] = {5, 2, 7, 1};
    glLightfv(GL_LIGHT0, GL_COLOR, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    //init material properties
    float ambient_color[] = {0, 1, 1, 1};
    float diff_color[] = {0.4, 0.3, 1, 1};
    float spec_color[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
    glColor4fv(ambient_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //initialize data to be passed to Marching Cubes
    mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
    MpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
    for(int i=0; i < nX+1; i++)
        for(int j=0; j < nY+1; j++)
            for(int k=0; k < nZ+1; k++) {
                mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
                vert.val = Potential((MpVector)vert);
                mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
            }
    //runs Marching Cubes
    Triangles = MarchingCubes(nX, nY, nZ, minValue, mcPoints, LinearInterp, numOfTriangles);
}

void GlWindow::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    makeCurrent();
    swapBuffers();
}

void GlWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = (float)h/(float)w;
    glFrustum(-1,1, -ratio, ratio, 1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GlWindow::mouseMoveEvent(QMouseEvent *pe)
{
    static int x = pe->x(), y = pe->y();
    {
        angleY += pe->x() < x ? -saY : (pe->x() == x ? 0 : saY);
        angleX += pe->y() < y ? -saX : (pe->y() == y ? 0 : saX);
        x = pe->x();
        y = pe->y();
    }
    updateGL();
}

void GlWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
        glTranslatef(transX, transY, transZ);
        glRotatef(angleX, 1, 0, 0);
        glRotatef(angleY, 0, 1, 0);
        glRotatef(angleZ, 0, 0, 1);
        //draws triangles
        glBegin(GL_TRIANGLES);
            for(int i=0; i < numOfTriangles; i++){
                glNormal3f(-Triangles[i].norm.x, -Triangles[i].norm.y, -Triangles[i].norm.z);
                for(int j=0; j < 3; j++)
                    glVertex3f(Triangles[i].p[j].x,Triangles[i].p[j].y,Triangles[i].p[j].z);
            }
        glEnd();
        //draws coordinates
        drawCoordinates(6);
    glPopMatrix();

    //tells OpenGL to execute the above commands
    //glFlush();
    //swapBuffers();
}

void GlWindow::InitData()
{
    delete [] mcPoints;	//first free the previous allocated memory
    mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
    MpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
    for(int i=0; i < nX+1; i++)
        for(int j=0; j < nY+1; j++)
            for(int k=0; k < nZ+1; k++) {
                mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
                vert.val = Potential((MpVector)vert);
                mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
    }
}


void GlWindow::RunMarchingCubesTest()
{
    delete [] Triangles;	//first free the previous allocated memory
    Triangles = MarchingCubes(nX, nY, nZ, minValue, mcPoints, LinearInterp, numOfTriangles);
}


float Potential(MpVector p)
{
    double r = (rand() % 10) /10.0;

    return (p.x * p.x + p.y*p.y + p.z*p.z) + r;
    return (pow(p.x, 3)/2 + pow(p.y, 3)/2 + pow(p.z, 3)/2);
    MpVector dp1 = MpVector( 0.0, -2.0,  0.0)-p;
    MpVector dp2 = MpVector( 0.0,  2.0,  0.0)-p;
    MpVector dp3 = MpVector( 2.0,  2.0,  0.0)-p;
    MpVector dp4 = MpVector( 0.0,  0.0,  4.0)-p;
    MpVector dp5 = MpVector(-0.5,  3.1, -1.0)-p;
    MpVector dp6 = MpVector( 0.0,  0.0, -4.0)-p;
    return 1/dp1.Magnitude() + 1/dp2.Magnitude() + 1/dp3.Magnitude() + 1/dp4.Magnitude() + 1/dp5.Magnitude() +
        1/dp6.Magnitude();
}

void drawCoordinates(float s)
{
    glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, 0.0);		//x coordinate
        glVertex3f(s, 0.0, 0.0);

        glVertex3f(0.0, 0.0, 0.0);		//y coordinate
        glVertex3f(0.0, s, 0.0);

        glVertex3f(0.0, 0.0, 0.0);		//z coordinate
        glVertex3f(0.0, 0.0, s);
    glEnd();
}


