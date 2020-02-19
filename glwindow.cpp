#include "glwindow.h"

GlWindow::GlWindow(QWidget *parent):
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}


void GlWindow::initializeGL()
{
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

    float ambient_color[] = {0, 1, 1, 1};
    float diff_color[] = {0.4, 0.3, 1, 1};
    float spec_color[] = {1, 1, 1, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
    glColor4fv(ambient_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    InitData();
    RunMarchingCubes();
}

void GlWindow::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    makeCurrent();
    swapBuffers();
}

void GlWindow::updateGL(bool needReculculate)
{
    try
    {
        if(needReculculate)
        {
            InitData();
            RunMarchingCubes();
        }
        QGLWidget::updateGL();

    }
    catch (int error)
    {
        QGLWidget::updateGL();
    }
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
        transX += 0.1;
        transY += 0.1;
        transZ += 0.1;
    }
    else if(pe->delta() > 0)
    {
        transX -= 0.1;
        transY -= 0.1;
        transZ -= 0.1;
    }

    updateGL();
}

void GlWindow::keyPressEvent(QKeyEvent *pe)
{
    switch (pe->key())
    {
        case Qt::Key_W:
            transZ += 0.5;
            break;
        case Qt::Key_S:
            transZ -= 0.5;
            break;
        case Qt::Key_A:
            transX += 0.5;
            break;
        case Qt::Key_D:
            transX -= 0.5;
            break;
        case Qt::Key_Q:
            transY += 0.5;
            break;
        case Qt::Key_E:
            transY -= 0.5;
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
                if(!flatten)
                {
                    glNormal3f(invert * Triangles[i].norm.x, invert * Triangles[i].norm.y, invert * Triangles[i].norm.z);
                }
                for(int j=0; j < 3; j++)
                    glVertex3f(Triangles[i].p[j].x,Triangles[i].p[j].y,Triangles[i].p[j].z);
            }
        glEnd();
    glPopMatrix();
}

void GlWindow::InitData()
{
    nX = (MAXX-MINX) / stepX;
    nY = (MAXY-MINY) / stepY;
    nZ = (MAXZ-MINZ) / stepZ;
    delete [] mcPoints;
    mcPoints = new mp4Vector[(nX+1)*(nY+1)*(nZ+1)];
    MpVector stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
    for(int i=0; i < nX+1; i++)
    {
        for(int j=0; j < nY+1; j++)
        {
            for(int k=0; k < nZ+1; k++)
            {
                mp4Vector vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
                vert.val = density_function((MpVector)vert);
                mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
            }
        }
    }
}


void GlWindow::RunMarchingCubes()
{
    delete [] Triangles;
    Triangles = MarchingCubes(nX, nY, nZ, isoVlaue, mcPoints, LinearInterp, numOfTriangles);
}

float GlWindow::density_function(MpVector p)
{
    float x, y, z;
    x = p.x;
    y = p.y;
    z = p.z;
    float u;
    float r, theta, phi;
    float Zet = 1.0, a;

    a = 1.0;

    r = sqrt(x*x + y*y +z*z);

    theta = atan(sqrt(x*x + y*y)/z);

    phi = atan(y/x);

    // n = 2, l = 1, m = 0; 2p
/*
    float A = pow(Zet/(2.0 * a), 1.5) * (2.0 - Zet*r/a);

    float R = exp(-Zet*r/(2.0*a));

    float Y = 0.5 * sqrt(3.0/ M_PI) * cos(theta);

    u = A*R*Y;

    // n = 3, l = 2, m = 0; dyz
*/
/*

    float A = pow(Zet/(3.0 * a), 1.5) * (4.0/(27.0*sqrt(10.0)));

    float R = (Zet*Zet*r*r/(a*a)) * exp(-Zet*r/(3.0*a));

    float Y = 0.5 * sqrt(15.0/ M_PI) * (y*z/(r*r));

    u = A*R*Y;

*/

    // n = 3, l = 2, m = 0; dz^2

/*

    float A = pow(Zet/(3.0 * a), 1.5) * (4.0/(27.0*sqrt(10.0)));

    float R = (Zet*Zet*r*r/(a*a)) * exp(-Zet*r/(3.0*a));

    float Y = 0.25 * sqrt(5.0/ M_PI) * ((-x*x - y*y + 2*z*z)/(r*r));

    u = A*R*Y;

*/

    // n = 3, l = 2, m = 0; dyz



    float A = pow(Zet/(3.0 * a), 1.5) * (4.0/(27.0*sqrt(10.0)));

    float R = (Zet*Zet*r*r/(a*a)) * exp(-Zet*r/(3.0*a));

    float Y = 0.5 * sqrt(15.0/ M_PI) * (y*z/(r*r));

    u = A*R*Y;



    // n = 3, l = 2, m = 0;



    u = pow(Zet/(3.0 * a), 1.5) * (4.0/(27.0*sqrt(10.0))) *

    (Zet*Zet*r*r/a*a) * exp(-Zet*r/(3.0*a))* 0.25 * sqrt(5.0/ M_PI) *

    (3*cos(theta)*cos(theta) - 1.0);



    return u*u * 10000; // квадрат модуля - плотность вероятности
}
