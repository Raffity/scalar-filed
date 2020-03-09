#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked);

    void on_pushButton_2_clicked(bool checked);

    void on_isovalue_editingFinished();

    void on_area_x_m_editingFinished();

    void on_area_y_m_editingFinished();

    void on_area_m_editingFinished();

    void on_area_x_p_editingFinished();

    void on_area_y_p_editingFinished();

    void on_area_z_p_editingFinished();

    void on_step_x_editingFinished();

    void on_step_y_editingFinished();

    void on_step_z_editingFinished();

    void on_checkBox_stateChanged(int arg1);

    void on_multiplier_editingFinished();

    void on_colorChange_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
