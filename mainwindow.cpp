#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->GRAPH_2D->hide();
    on_pushButton_Figure_1_clicked();
    on_pushButton_2D_clicked();

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


        int numPoints = 100;
        for (int i = 0; i < numPoints; ++i)
        {
            double x = i;
            double y = 10 * std::sin(0.1 * i);
            series->append(x, y);
        }

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

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();

    int gridSizeX = 50;
    int gridSizeY = 50;
    double spacingX = 1.0;
    double spacingY = 1.0;

    for (int i = 0; i < gridSizeX; i++) {
        for (int j = 0; j < gridSizeY; j++) {
            double x = i * spacingX;
            double y = j * spacingY;
            double z = sin(sqrt(x * x + y * y * 2)) * 2;
            points->InsertNextPoint(x, y, z);
            scalars->InsertNextValue(z);
        }
    }

    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->GetPointData()->SetScalars(scalars);

    vtkSmartPointer<vtkDelaunay2D> delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
    delaunay->SetInputData(polyData);
    delaunay->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(delaunay->GetOutputPort());
    mapper->SetScalarRange(polyData->GetPointData()->GetScalars()->GetRange());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);


    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->qvtkWidget_GRAPH->setRenderWindow(renderWindow);
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    renderWindow->GetInteractor()->SetInteractorStyle(style);


    vtkSmartPointer<vtkAxesActor> orientationAxes = vtkSmartPointer<vtkAxesActor>::New();
    orientationAxes->SetTotalLength(10.0, 10.0, 10.0);

    vtkSmartPointer<vtkOrientationMarkerWidget> widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOrientationMarker(orientationAxes);
    widget->SetInteractor(renderWindow->GetInteractor());
    widget->SetViewport(0.0, 0.0, 0.2, 0.2);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    vtkSmartPointer<vtkCubeAxesActor2D> cubeAxes = vtkSmartPointer<vtkCubeAxesActor2D>::New();
    cubeAxes->SetInputData(delaunay->GetOutput());
    cubeAxes->SetFontFactor(3.0);
    cubeAxes->SetFlyModeToNone();
    cubeAxes->SetCamera(renderer->GetActiveCamera());

    vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
    camera->SetPosition(70, -50, 25);
    camera->SetFocalPoint(25, 25, 0);
    camera->SetViewUp(0, 0, 1);
    renderer->ResetCameraClippingRange();



    vtkSmartPointer<vtkAxisActor2D> xAxis = cubeAxes->GetXAxisActor2D();
    xAxis->SetAdjustLabels(1);

    renderer->AddViewProp(cubeAxes);
    renderWindow->Render();

}

void MainWindow::on_pushButton_Figure_1_clicked()
{
    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_pos_x->setText("X");
    ui->label_pos_x->show();
    ui->label_pos_y->setText("Y");
    ui->label_pos_y->show();
    ui->label_pos->setText("Расположение");
    ui->lineEdit_pos_x->show();
    ui->lineEdit_pos_y->show();
    ui->lineEdit_col_horizontally->hide();
    ui->lineEdit_col_vertically->hide();
    ui->lineEdit_pos_horizontally->hide();
    ui->lineEdit_pos_vertically->hide();
    ui->label_pos->show();

    ui->label_aperture_height->setText("Высота");
    ui->label_aperture_width->show();
    ui->lineEdit_aperture_width->show();

    ui->label_horizontally->hide();
    ui->label_vertically->hide();
    ui->label_col->hide();
    rectangleSource = vtkSmartPointer<vtkCubeSource>::New();
    double width = ui->lineEdit_size_d->text().toDouble();      //Y
    double height = ui->lineEdit_size_b->text().toDouble();     //Z
    double length = ui->lineEdit_size_a->text().toDouble();     //X

    rectangleSource->SetYLength(height);
    rectangleSource->SetXLength(length);
    rectangleSource->SetZLength(width);
    rectangleSource->Update();

    vtkNew<vtkCubeSource> notchSource;
    double notchWidth = 0.002;                                              //Y
    double notchHeight = ui->lineEdit_aperture_height->text().toDouble();   //Z - в нормальной системе координат
    double notchDepth = ui->lineEdit_aperture_width->text().toDouble();     //X

    double X_shift = ui->lineEdit_pos_x->text().toDouble();
    double Y_shift = ui->lineEdit_pos_y->text().toDouble();


    ///{ Если размеры выходят за рамки модели
    if (X_shift + notchDepth >= length && X_shift != 0)
    {
        X_shift = 0.0001;
        notchDepth = length / 4;

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::red);

        ui->lineEdit_pos_x->setPalette(*palette);           // X shift
        ui->lineEdit_aperture_width->setPalette(*palette);  // NotchDepth
        ui->lineEdit_size_a->setPalette(*palette);          // Length

        goto after_error;
    }
    else
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);

        ui->lineEdit_pos_x->setPalette(*palette);
        ui->lineEdit_aperture_width->setPalette(*palette);
        ui->lineEdit_size_a->setPalette(*palette);
    }

    if (Y_shift + notchHeight >= height && Y_shift != 0)
    {
        Y_shift = 0.0001;
        notchHeight = height / 4;

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::red);

        ui->lineEdit_pos_y->setPalette(*palette);           //Y shift
        ui->lineEdit_aperture_height->setPalette(*palette); // NotchHeight
        ui->lineEdit_size_b->setPalette(*palette);          // Height

        goto after_error;
    }
    else
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);

        ui->lineEdit_pos_y->setPalette(*palette);
        ui->lineEdit_aperture_height->setPalette(*palette);
        ui->lineEdit_size_b->setPalette(*palette);
    }

    if (notchHeight >= height)
    {
        notchHeight = height / 4;

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::red);

        ui->lineEdit_aperture_height->setPalette(*palette); // NotchHeight
        ui->lineEdit_size_b->setPalette(*palette);          // Height

        goto after_error;
    }
    else
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);

        ui->lineEdit_aperture_height->setPalette(*palette);
        ui->lineEdit_size_b->setPalette(*palette);
    }

    if (notchDepth >= length)
    {
        notchDepth = length / 4;

        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::red);

        ui->lineEdit_aperture_width->setPalette(*palette);  // NotchDepth
        ui->lineEdit_size_a->setPalette(*palette);          // Length

        goto after_error;
    }
    else
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::white);

        ui->lineEdit_aperture_width->setPalette(*palette);
        ui->lineEdit_size_a->setPalette(*palette);
    }
    ///}

after_error:

    notchSource->SetYLength(notchHeight);
    notchSource->SetXLength(notchDepth);
    notchSource->SetZLength(notchWidth);
    notchSource->Update();

    vtkNew<vtkTransform> notchTransform;
    notchTransform->Translate(-(length / 2) + (notchDepth / 2) + X_shift /*X*/, -(height / 2) + (notchHeight / 2) + Y_shift /*Z*/, (width / 2) - 0.00099 /*Y*/ );

    vtkNew<vtkTransformPolyDataFilter>notchTransformFilter;
    notchTransformFilter->SetInputConnection(notchSource->GetOutputPort());
    notchTransformFilter->SetTransform(notchTransform);
    notchTransformFilter->Update();
    vtkNew<vtkPolyDataMapper> notchMapper;
    notchMapper->SetInputConnection(notchTransformFilter->GetOutputPort());

    vtkNew<vtkActor> notchActor;
    notchActor->SetMapper(notchMapper);
    notchActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(rectangleSource->GetOutputPort());
    vtkNew<vtkActor> rectangleActor;
    rectangleActor->SetMapper(mapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("White").GetData());
    renderer->AddActor(rectangleActor);
    renderer->AddActor(notchActor);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}


void MainWindow::on_pushButton_Figure_2_clicked()
{
    figure = 2;
    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_pos_x->setText("Гор.");
    ui->label_pos_y->setText("Верт.");
    ui->label_pos->setText("Расстояние между центрами апертур");
    ui->lineEdit_pos_x->hide();
    ui->lineEdit_pos_y->hide();
    ui->lineEdit_col_horizontally->show();
    ui->lineEdit_col_vertically->show();
    ui->lineEdit_pos_horizontally->show();
    ui->lineEdit_pos_vertically->show();
    ui->label_horizontally->show();
    ui->label_vertically->show();
    ui->label_col->show();

    rectangleSource = vtkSmartPointer<vtkCubeSource>::New();
    double width = ui->lineEdit_size_d->text().toDouble();      // Y
    double height = ui->lineEdit_size_b->text().toDouble();     // Z
    double length = ui->lineEdit_size_a->text().toDouble();     // X

    rectangleSource->SetYLength(height);
    rectangleSource->SetXLength(length);
    rectangleSource->SetZLength(width);
    rectangleSource->Update();

    vtkNew<vtkCubeSource> notchSource;

    double notchWidth = ui->lineEdit_aperture_width->text().toDouble();
    double notchHeight = ui->lineEdit_aperture_height->text().toDouble();
    double notchDepth = 0.003;

    int rows = ui->lineEdit_col_vertically->text().toInt();
    int cols = ui->lineEdit_col_horizontally->text().toInt();

    double horizontalSpacing = ui->lineEdit_pos_horizontally->text().toDouble();
    double verticalSpacing = ui->lineEdit_pos_vertically->text().toDouble();

    double totalGridWidth = (cols - 1) * horizontalSpacing + notchWidth;
    double totalGridHeight = (rows - 1) * verticalSpacing + notchHeight;

    bool exceedsWidth = totalGridWidth > length;
    bool exceedsHeight = totalGridHeight > height;
    bool intersectionDetected = false;

    if (notchHeight >= verticalSpacing) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_pos_vertically->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_pos_vertically->setPalette(palette);
    }

    if (notchWidth >= horizontalSpacing) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_width->setPalette(palette);
        ui->lineEdit_pos_horizontally->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_width->setPalette(palette);
        ui->lineEdit_pos_horizontally->setPalette(palette);
    }


    if (exceedsWidth) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_col_horizontally->setPalette(palette);
        ui->lineEdit_pos_horizontally->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_col_horizontally->setPalette(palette);
        ui->lineEdit_pos_horizontally->setPalette(palette);
    }

    if (exceedsHeight) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_col_vertically->setPalette(palette);
        ui->lineEdit_pos_vertically->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_col_vertically->setPalette(palette);
        ui->lineEdit_pos_vertically->setPalette(palette);
    }

    if (notchWidth >= length) {
        notchWidth = length / 4;
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_width->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_width->setPalette(palette);
    }

    if (notchHeight >= height) {
        notchHeight = height / 4;
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_height->setPalette(palette);
        intersectionDetected = true;
        goto after_error;
    } else {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_height->setPalette(palette);
    }
after_error:
    if (intersectionDetected) {
        return;
    }

    notchSource->SetYLength(notchHeight);
    notchSource->SetXLength(notchWidth);
    notchSource->SetZLength(notchDepth);
    notchSource->Update();

    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(rectangleSource->GetOutputPort());
    vtkNew<vtkActor> rectangleActor;
    rectangleActor->SetMapper(mapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    renderer->AddActor(rectangleActor);

    double gridOffsetX = -(totalGridWidth / 2.0) + (notchWidth / 2.0);
    double gridOffsetY = -(totalGridHeight / 2.0) + (notchHeight / 2.0);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            double notchPosX = gridOffsetX + col * horizontalSpacing;
            double notchPosY = gridOffsetY + row * verticalSpacing;

            vtkSmartPointer<vtkTransform> notchTransform = vtkSmartPointer<vtkTransform>::New();
            notchTransform->Translate(notchPosX, notchPosY, (width / 2.0) - 0.001);

            vtkSmartPointer<vtkTransformPolyDataFilter> notchTransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            notchTransformFilter->SetInputConnection(notchSource->GetOutputPort());
            notchTransformFilter->SetTransform(notchTransform);
            notchTransformFilter->Update();

            vtkNew<vtkPolyDataMapper> notchMapper;
            notchMapper->SetInputConnection(notchTransformFilter->GetOutputPort());

            vtkNew<vtkActor> notchActor;
            notchActor->SetMapper(notchMapper);
            notchActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

            renderer->AddActor(notchActor);
        }
    }

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}


void MainWindow::on_pushButton_Figure_3_clicked()
{
    figure = 3;
    ui->lineEdit_size_d->hide();
    ui->label_size_d->hide();
    ui->label_size_a->setText("h");
    ui->label_size_b->setText("r");
    ui->label_aperture_height->setText("Радиус");
    ui->label_aperture_width->hide();
    ui->lineEdit_aperture_width->hide();

    ui->lineEdit_col_horizontally->hide();
    ui->lineEdit_col_vertically->hide();
    ui->lineEdit_pos_horizontally->hide();
    ui->lineEdit_pos_vertically->hide();
    ui->label_pos_x->hide();
    ui->label_pos_y->hide();
    ui->lineEdit_pos_x->hide();
    ui->lineEdit_pos_y->hide();

    ui->label_col->hide();
    ui->lineEdit_col_vertically->hide();
    ui->lineEdit_col_horizontally->hide();
    ui->label_horizontally->hide();
    ui->label_vertically->hide();
    ui->label_pos->hide();

    double radius = ui->lineEdit_size_b->text().toDouble();
    double height = ui->lineEdit_size_a->text().toDouble();

    double notchRadius = ui->lineEdit_aperture_height->text().toDouble();
    double notchHeight = height / 2 ;

    if(notchRadius >= radius)
    {
        notchRadius = radius / 4;
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_size_b->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_size_b->setPalette(palette);
    }

    cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetResolution(40);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinderSource->GetOutputPort());

    cylinderSource->SetHeight(height);
    cylinderSource->SetRadius(radius);
    cylinderSource->Update();


    vtkNew<vtkCylinderSource> notchSource;
    notchSource->SetResolution(40);

    notchSource->SetHeight(notchHeight);
    notchSource->SetRadius(notchRadius);
    notchSource->Update();

    vtkNew<vtkTransform> notchTransform;
    notchTransform->Translate(0, -(height / 2 - notchHeight / 2+ 0.001), 0);
    notchTransform->Update();

    vtkNew<vtkTransformPolyDataFilter> notchTransformFilter;
    notchTransformFilter->SetInputConnection(notchSource->GetOutputPort());
    notchTransformFilter->SetTransform(notchTransform);
    notchTransformFilter->Update();

    vtkNew<vtkPolyDataMapper> notchMapper;
    notchMapper->SetInputConnection(notchTransformFilter->GetOutputPort());

    vtkNew<vtkActor> notchActor;
    notchActor->SetMapper(notchMapper);
    notchActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinderSource->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());


    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(cylinderActor);
    renderer->AddActor(notchActor);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();

}


void MainWindow::on_lineEdit_size_a_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    // if(arg1.toDouble() <= ui->lineEdit_aperture_width->text().toDouble()){
    //     QPalette *palette = new QPalette();
    //     palette->setColor(QPalette::Base,Qt::red);
    //     return;
    // }
    // else{
    //     QPalette *palette = new QPalette();
    //     palette->setColor(QPalette::Base,Qt::white);
    //     ui->lineEdit_size_a->setPalette(*palette);
    // }

    double length = arg1.toDouble();
    switch (figure) {
    case 3:
        on_pushButton_Figure_3_clicked();
        break;
    case 2:
        rectangleSource->SetXLength(length);
        rectangleSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    case 1:
        on_pushButton_Figure_1_clicked();
        break;
    }
}


void MainWindow::on_lineEdit_size_b_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    // if(arg1.toDouble() <= ui->lineEdit_aperture_height->text().toDouble()){
    //     QPalette *palette = new QPalette();
    //     palette->setColor(QPalette::Base,Qt::red);
    //     ui->lineEdit_size_b->setPalette(*palette);
    //     return;
    // }
    // else{
    //     QPalette *palette = new QPalette();
    //     palette->setColor(QPalette::Base,Qt::white);
    //     ui->lineEdit_size_b->setPalette(*palette);
    // }

    double height = arg1.toDouble();
    switch (figure) {
    case 3:
        on_pushButton_Figure_3_clicked();
        break;
    case 2:
        rectangleSource->SetYLength(height);
        rectangleSource->Update();
        ui->qvtkWidget_3D_MODEL->renderWindow()->Render();
        ui->qvtkWidget_3D_MODEL->update();
        break;
    case 1:
        on_pushButton_Figure_1_clicked();
        break;

    }
}


void MainWindow::on_lineEdit_size_d_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    //double width = arg1.toDouble();
    switch (figure)
    {
    case 2:
        on_pushButton_Figure_2_clicked();
        break;
    case 1:
        on_pushButton_Figure_1_clicked();
        break;
    }
}


void MainWindow::on_lineEdit_pos_x_textChanged(const QString &arg1)
{
    on_pushButton_Figure_1_clicked();
}


void MainWindow::on_lineEdit_pos_y_textChanged(const QString &arg1)
{
    on_pushButton_Figure_1_clicked();
}


void MainWindow::on_lineEdit_aperture_height_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;
    switch (figure)
    {
    case 3:
        on_pushButton_Figure_3_clicked();
        break;
    case 2:
        on_pushButton_Figure_2_clicked();
        break;
    case 1:
        on_pushButton_Figure_1_clicked();
        break;
    }

}


void MainWindow::on_lineEdit_aperture_width_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure)
    {
    case 2:
        on_pushButton_Figure_2_clicked();
        break;
    case 1:
        on_pushButton_Figure_1_clicked();
        break;
    }

}


void MainWindow::on_lineEdit_col_horizontally_textChanged(const QString &arg1)//количество по горизонтале
{
    if(arg1.toDouble() <= 0)
        return;

    on_pushButton_Figure_2_clicked();
}


void MainWindow::on_lineEdit_col_vertically_textChanged(const QString &arg1)//количество по вертикале
{
    if(arg1.toDouble() <= 0)
        return;

    on_pushButton_Figure_2_clicked();
}


void MainWindow::on_lineEdit_pos_horizontally_textChanged(const QString &arg1)//количество по горизонтале
{
    if(arg1.toDouble() <= 0)
        return;

    on_pushButton_Figure_2_clicked();
}


void MainWindow::on_lineEdit_pos_vertically_textChanged(const QString &arg1)//растояние  по вертикале
{
    if(arg1.toDouble() <= 0)
        return;

    on_pushButton_Figure_2_clicked();
}
