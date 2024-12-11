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
#include <QMessageBox>
#include <vtkTransformPolyDataFilter.h>

#include <QDebug>
#include <cmath>

#include "modelsfigure.h"
#include "calculationthread.h"
#include "herzformatter.h"
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkBox.h>
#include <vtkTransform.h>

//график
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkDelaunay2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkSimplePointsReader.h>
#include <vtkWarpScalar.h>
#include <vtkAxisActor2D.h>
#include <vtkCubeAxesActor2D.h>

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataArray>
#include <QtDataVisualization/QSurfaceDataRow>
#include <QtDataVisualization/QSurfaceDataItem>
#include <QHBoxLayout>
#include <QWidget>
#include <QVector3D>
#include <QVector>
#include <QSize>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Create2DGraph(int num);
    void Create3DGraph();

private slots:
    ///Графики
    void on_pushButton_2D_clicked();
    void on_pushButton_3D_clicked();

    ///Переключение фигур
    void on_pushButton_Figure_3_clicked();
    void on_pushButton_Figure_1_clicked();
    void on_pushButton_Figure_2_clicked();

    void create_figure_1();
    void create_figure_2();
    void create_figure_3();

    ///Изменение геометрии
    void on_lineEdit_size_a_textChanged(const QString &arg1);
    void on_lineEdit_size_b_textChanged(const QString &arg1);
    void on_lineEdit_size_d_textChanged(const QString &arg1);

    void on_lineEdit_pos_x_textChanged(const QString &arg1);

    void on_lineEdit_pos_y_textChanged(const QString &arg1);

    void on_lineEdit_aperture_height_textChanged(const QString &arg1);

    void on_lineEdit_aperture_width_textChanged(const QString &arg1);

    void on_lineEdit_col_horizontally_textChanged(const QString &arg1);

    void on_lineEdit_col_vertically_textChanged(const QString &arg1);

    void on_lineEdit_pos_horizontally_textChanged(const QString &arg1);

    void on_lineEdit_pos_vertically_textChanged(const QString &arg1);

    void on_pushButtonCalcStart_clicked();

    void CalcRAM();
    void CalcTime();
    void CalcDif();

    void UpdateProgress(double val);
    void PrintCalcTime(double val);
    void PrintCalcIter(double val);
    void UpdateGUI(QVector<surfaceModelItem> gui);

    void PointSelected(const QPoint &position);

    void on_lineEdit_POV_NofPoints_textChanged(const QString &arg1);

    void on_lineEdit_Source_NofPoints_textChanged(const QString &arg1);

    void on_comboBox_func_currentTextChanged(const QString &arg1);

    void on_lineEdit_POV_P_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    bool graph_2d_exists = false;
    QLineSeries *series;
    QChartView *chartView;
    int figure;

    vtkNew<vtkNamedColors> colors;
    ModelsFigure modelsFigure;
    CalculationThread* calc_thread;
    QVector<surfaceModelItem> mItems;

    QSurface3DSeries *series1;
    QChart *chart;
    Q3DSurface *surface;
    QWidget *container;

    HerzFormatter *hzf;

    bool error_occured = false;

    int RAM;
    int Difficulty;
    double Time;

};

#endif // MAINWINDOW_H
