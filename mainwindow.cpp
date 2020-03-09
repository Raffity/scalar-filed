#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->invert = -1;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    ui->openGLWidget->invert *= -1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_pushButton_2_clicked(bool checked)
{
     ui->openGLWidget->flatten = ui->openGLWidget->flatten ? false : true;
     ui->openGLWidget->updateGL();
}

void MainWindow::on_isovalue_editingFinished()
{
    ui->openGLWidget->isoVlaue = ui->isovalue->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_area_x_m_editingFinished()
{
    ui->openGLWidget->MINX = ui->area_x_m->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_area_y_m_editingFinished()
{
    ui->openGLWidget->MINY = ui->area_y_m->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_area_m_editingFinished()
{
    ui->openGLWidget->MINZ = ui->area_z_m->value();
    ui->openGLWidget->updateGL(true);

}

void MainWindow::on_area_x_p_editingFinished()
{
    ui->openGLWidget->MAXX = ui->area_x_p->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_area_y_p_editingFinished()
{
    ui->openGLWidget->MAXY = ui->area_y_p->value();
    ui->openGLWidget->updateGL(true);

}

void MainWindow::on_area_z_p_editingFinished()
{
    ui->openGLWidget->MAXZ = ui->area_z_p->value();
    ui->openGLWidget->updateGL(true);

}

void MainWindow::on_step_x_editingFinished()
{
    ui->openGLWidget->stepX = ui->step_x->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_step_y_editingFinished()
{
    ui->openGLWidget->stepY = ui->step_y->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_step_z_editingFinished()
{
    ui->openGLWidget->stepZ = ui->step_z->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 > 0)
    {
        ui->openGLWidget->setSharedName(ui->sharedName->text());
        ui->openGLWidget->startCheckSharedMemory();
    }
    else
    {
        ui->openGLWidget->stopCheckSharedMemory();
    }
}

void MainWindow::on_multiplier_editingFinished()
{
    ui->openGLWidget->multiplier = ui->multiplier->value();
    ui->openGLWidget->updateGL(true);
}

void MainWindow::on_colorChange_clicked()
{
    QColorDialog *colorDialog = new QColorDialog();
    colorDialog->show();
    colorDialog->exec();
    ui->openGLWidget->diffuseColor = colorDialog->selectedColor();
    ui->openGLWidget->updateGL();
}
