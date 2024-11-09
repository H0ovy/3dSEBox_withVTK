// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkNamedColors.h>
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkCubeSource.h>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <vtkActor.h>
#include <vtkTransformPolyDataFilter.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    ///Графики
    void on_pushButton_2D_clicked();
    void on_pushButton_3D_clicked();

    ///Переключение фигур
    void on_pushButton_Figure_3_clicked();
    void on_pushButton_Figure_1_clicked();
    void on_pushButton_Figure_2_clicked();

    ///Изменение геометрии
    void on_lineEdit_size_a_textChanged(const QString &arg1);
    void on_lineEdit_size_b_textChanged(const QString &arg1);
    void on_lineEdit_size_d_textChanged(const QString &arg1);

    void on_lineEdit_pos_x_textChanged(const QString &arg1);

    void on_lineEdit_pos_y_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkCylinderSource> cylinderSource;
    vtkSmartPointer<vtkCubeSource> rectangleSource;
    vtkSmartPointer<vtkActor> cylinderActor;

    vtkSmartPointer<vtkTransform> notchTransform;
    vtkSmartPointer<vtkTransformPolyDataFilter> notchTransformFilter;

    void updateCylinder(double radius, double height);
    bool graph_2d_exists = false;
    QLineSeries *series;
    QChartView *chartView;
    int figure;

    vtkNew<vtkNamedColors> colors;
};

#endif // MAINWINDOW_H
