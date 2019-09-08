#include "glwindow.h"

GlWindow::GlWindow(QWidget *parent):
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}


void GlWindow::initializeGL()
{
    stepZ = 0.5;
    stepX = 0.5;
    stepY = 0.5;
    transZ = -8;
    transX =0;
    transY = 0;
    angleX= 0;
    angleY = 0;
    angleZ = 0;
    saX = 2;
    saY = 2;
    saZ = 2;
    minValue = 1;
    nX = 20;
    nY = 20;
    nZ = 20;
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float light_color[] = {1, 1, 1, 1};
    float light_position[] = {5, 2, 7, 1};
    float light_position1[] = {-50, -20, -70, 1};
    glLightfv(GL_LIGHT0, GL_COLOR, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_COLOR, light_color);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);

    float ambient_color[] = {0, 1, 1, 1};
    float diff_color[] = {0.4, 0.3, 1, 1};
    float spec_color[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
    glColor4fv(ambient_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
    MpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
    for(int i=0; i < nX+1; i++)
        for(int j=0; j < nY+1; j++)
            for(int k=0; k < nZ+1; k++) {
                mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
                vert.val = culculateFunc((MpVector)vert);
                mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
            }

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

void GlWindow::wheelEvent(QWheelEvent *pe)
{
    if(pe->delta() < 0)
    {
        transX += 0.5;
        transY += 0.5;
        transZ += 0.5;
    }
    else if(pe->delta() > 0)
    {
        transX -= 0.5;
        transY -= 0.5;
        transZ -= 0.5;
    }

    updateGL();
}

void GlWindow::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
        //Move camera
        case Qt::Key_W:
            transZ += 2;
            break;
        case Qt::Key_S:
            transZ -= 2;
            break;
        case Qt::Key_A:
            transX += 2;
            break;
        case Qt::Key_D:
            transX -= 2;
            break;
        case Qt::Key_Q:
            transY += 2;
            break;
        case Qt::Key_E:
            transY -= 2;
            break;
            //Change number split
        case Qt::Key_T:
            nX < 150 ? nX+=10 : nX; InitData(); RunMarchingCubesTest();
            break;
        case Qt::Key_G:
            nX > 1? nX-=10 : nX; InitData(); RunMarchingCubesTest();
            break;
        case Qt::Key_Y:
            nY < 150 ? nY+=10: nY; InitData(); RunMarchingCubesTest();
            break;
        case Qt::Key_H:
            nY > 1? nY-=10 : nY; InitData(); RunMarchingCubesTest();
            break;
        case Qt::Key_U:
            nZ < 150 ? nZ+=10: nZ; InitData(); RunMarchingCubesTest();
            break;
        case Qt::Key_J:
            nZ > 1? nZ-=10 : nZ; InitData(); RunMarchingCubesTest();
            break;
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
        glBegin(GL_TRIANGLES);
            for(int i=0; i < numOfTriangles; i++){
                glNormal3f(Triangles[i].norm.x, Triangles[i].norm.y, Triangles[i].norm.z);
                for(int j=0; j < 3; j++)
                    glVertex3f(Triangles[i].p[j].x,Triangles[i].p[j].y,Triangles[i].p[j].z);
            }
        glEnd();
    glPopMatrix();
}

void GlWindow::InitData()
{
    delete [] mcPoints;
    mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
    MpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
    for(int i=0; i < nX+1; i++)
        for(int j=0; j < nY+1; j++)
            for(int k=0; k < nZ+1; k++) {
                mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
                vert.val = culculateFunc((MpVector)vert);
                mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
    }
}


void GlWindow::RunMarchingCubesTest()
{
    delete [] Triangles;
    Triangles = MarchingCubes(nX, nY, nZ, minValue, mcPoints, LinearInterp, numOfTriangles);
}


float culculateFunc(MpVector p)
{
    //return sin(p.x*p.x + p.y * p.y)/(p.x * p.x+p.y*p.y) - p.z;
    //return (p.x * p.x + p.y*p.y + p.z*p.z);
    return sin(0.1*p.x*p.x+0.2*p.y*p.y) - p.z;
    return (pow(p.x, 3)/2 + pow(p.y, 3)/2 + pow(p.z, 3)/2);
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


