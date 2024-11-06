#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QDebug>
#include <vtkProperty.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->GRAPH_2D->hide();
    on_pushButton_Figure_2_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2D_clicked()
{
    ui->qvtkWidget_GRAPH->hide();
    if (graph_2d_exists)
    {
        ui->GRAPH_2D->show();
    }
    else
    {
        series = new QLineSeries();
        QChart *chart = new QChart();

        series->setPen(QPen(Qt::blue, 2, Qt::SolidLine));
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setAnimationOptions(QChart::AllAnimations);

        QValueAxis *axisX = new QValueAxis;
        axisX->setTitleText("Частота, Гц");
        axisX->setTitleFont(QFont("Times New Roman", 12, QFont::Bold));
        axisX->setLabelsFont(QFont("Arial", 10));
        axisX->setLabelFormat("%d");
        axisX->setTickCount(10);
        chart->setAxisX(axisX, series);

        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("ЭЭ, дБ");
        axisY->setTitleFont(QFont("Times New Roman", 12, QFont::Bold));
        axisY->setLabelsFont(QFont("Arial", 10));
        axisY->setLabelFormat("%.2f");
        axisY->setTickCount(10);
        chart->setAxisY(axisY, series);

        ui->GRAPH_2D->setChart(chart);
        ui->GRAPH_2D->setRenderHint(QPainter::Antialiasing);
        chart->legend()->hide();
        ui->GRAPH_2D->show();

        graph_2d_exists = true;
    }
}

void MainWindow::on_pushButton_3D_clicked()
{
    ui->qvtkWidget_GRAPH->show();
    ui->GRAPH_2D->hide();
}

void MainWindow::on_pushButton_Figure_1_clicked()
{
    figure = 1;
    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
}

void MainWindow::on_pushButton_Figure_2_clicked()
{
    figure = 2;
    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    rectangleSource = vtkSmartPointer<vtkCubeSource>::New();


    double width = ui->lineEdit_size_d->text().toDouble();
    double height = ui->lineEdit_size_b->text().toDouble();
    double length = ui->lineEdit_size_a->text().toDouble();

    rectangleSource->SetYLength(height);
    rectangleSource->SetXLength(length);
    rectangleSource->SetZLength(width);
    rectangleSource->Update();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(rectangleSource->GetOutputPort());
    vtkSmartPointer<vtkActor> rectangleActor = vtkSmartPointer<vtkActor>::New();
    rectangleActor->SetMapper(mapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(rectangleActor);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    rectangleSource->Update();

    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}

void MainWindow::on_pushButton_Figure_3_clicked()
{
    figure = 3;
    ui->lineEdit_size_d->hide();
    ui->label_size_d->hide();
    ui->label_size_a->setText("r");
    ui->label_size_b->setText("h");
    cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetResolution(8);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinderSource->GetOutputPort());

    cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(mapper);
    cylinderActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(cylinderActor);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    double radius = ui->lineEdit_size_b->text().toDouble();
    double height = ui->lineEdit_size_a->text().toDouble();

    cylinderSource->SetHeight(height);
    cylinderSource->SetRadius(radius);
    cylinderSource->Update();

    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}

void MainWindow::on_lineEdit_size_b_selectionChanged()
{
    double height = ui->lineEdit_size_b->text().toDouble();
    switch (figure) {
    case 3:
        cylinderSource->SetHeight(height);
        cylinderSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    case 2:
        rectangleSource->SetYLength(height);
        rectangleSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    }
}


void MainWindow::on_lineEdit_size_a_selectionChanged()
{
    double length = ui->lineEdit_size_a->text().toDouble();
    switch (figure) {
    case 3:
        cylinderSource->SetHeight(length);
        cylinderSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    case 2:
        rectangleSource->SetXLength(length);
        rectangleSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    }

}


void MainWindow::on_lineEdit_size_d_selectionChanged()
{
    double width = ui->lineEdit_size_d->text().toDouble();

    switch (figure) {
    case 2:
        rectangleSource->SetZLength(width);
        rectangleSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    case 1:
        break;
    }
}

