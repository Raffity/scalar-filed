#include "glwindow.h"
#include <QOpenGLShader>

GlWindow::GlWindow(QWidget *parent):
    QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}


void GlWindow::initializeGL()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
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

    float ambient_color[] = {1, 1, 1, 1};
    float diff_color[] = {1, 0.1, 0.1, 1};
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

void GlWindow::startCheckSharedMemory()
{
    SharedMemoryChecker *checker = new SharedMemoryChecker(nullptr, this->sharedMemoryName);
    checker->moveToThread(sharedMemoryThread);
    connect(checker, SIGNAL(send(QList<mp4Vector>)), this, SLOT(updateDataOfPoints(QList<mp4Vector>)),Qt::DirectConnection);
    connect(checker, SIGNAL(sendSettings(int ,int, int, float, float, float, float, float, float)),
            this, SLOT(setSettingsArea(int ,int, int, float, float, float, float, float, float)),Qt::DirectConnection);
    connect(sharedMemoryThread, SIGNAL(started()), checker, SLOT(work()), Qt::DirectConnection);
    sharedMemoryThread->start();
}

void GlWindow::stopCheckSharedMemory()
{
    sharedMemoryThread->exit();
}


void GlWindow::updateGL(bool needReculculate)
{
    if(active)
    {
        if(needReculculate)
        {
            InitData();
            RunMarchingCubes();
        }
        float difColor[] = {(float)diffuseColor.red()/255, (float)this->diffuseColor.green()/255, (float)this->diffuseColor.blue()/255, (float)this->diffuseColor.alpha()/255};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difColor);
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
        //Move camera
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

void GlWindow::setSharedName(QString sharedName)
{
    if(sharedName.length() > 1)
    {
        this->sharedMemoryName = "Global\\" + sharedName;
    }
    else
    {
        this->sharedMemoryName ="";
    }
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
        for(int i=0; i < numOfTriangles; i++)
        {
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

void GlWindow::setSettingsArea(int nX, int nY, int nZ, float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
    GlWindow::nX = nX;
    GlWindow::nY = nY;
    GlWindow::nZ = nZ;
    GlWindow::MAXX = maxX;
    GlWindow::MAXY = maxY;
    GlWindow::MAXZ = maxZ;
    GlWindow::MINX = minX;
    GlWindow::MINY = minY;
    GlWindow::MINZ = minZ;

}

void GlWindow::updateDataOfPoints(QList<mp4Vector> points)
{
    active = false;
    GlWindow::oldPoints = points;
    this->InitData(points);
    this->RunMarchingCubes();
    active = true;
}

void GlWindow::InitData(QList<mp4Vector> points)
{
    delete [] mcPoints;

    if(points.size() > 1)
    {
        mcPoints = new mp4Vector[points.size()];
        int indx = 0;
        foreach(mp4Vector point, points)
        {
            mcPoints[indx] = point;
            indx++;
        }
    }
    else if(oldPoints.size() > 1)
    {
        mcPoints = new mp4Vector[oldPoints.size()];
        int indx = 0;
        foreach(mp4Vector point, oldPoints)
        {
            mcPoints[indx] = point;
            indx++;
        }
    }
    else
    {
        nX = (MAXX-MINX) / stepX;
        nY = (MAXY-MINY) / stepY;
        nZ = (MAXZ-MINZ) / stepZ;
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

    float A = pow(Zet/(2.0 * a), 1.5) * (2.0 - Zet*r/a);

    float R = exp(-Zet*r/(2.0*a));

    float Y = 0.5 * sqrt(3.0/ M_PI) * cos(theta);

    u = A*R*Y;

    return u*u * this->multiplier; // квадрат модуля - плотность вероятности
}
