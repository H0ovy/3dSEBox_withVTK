#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar_calc->setRange(0, 6666);

    ui->GRAPH_2D->hide();
    on_pushButton_Figure_1_clicked();
    on_pushButton_2D_clicked();


    //calc_thread = new CalculationThread();
    CalcRAM();
    CalcTime();
    CalcDif();
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
        chart->setAnimationOptions(QChart::AllAnimations);

        QValueAxis *axisX = new QValueAxis;
        axisX->setTitleText("Частота, Гц");
        axisX->setTitleFont(QFont("Times New Roman", 12, QFont::Bold));
        axisX->setLabelsFont(QFont("Arial", 10));
        axisX->setLabelFormat("%d");
        axisX->setTickCount(10);
        chart->addAxis(axisX, Qt::AlignBottom);

        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("ЭЭ, дБ");
        axisY->setTitleFont(QFont("Times New Roman", 12, QFont::Bold));
        axisY->setLabelsFont(QFont("Arial", 10));
        axisY->setLabelFormat("%.2f");
        axisY->setTickCount(10);
        chart->addAxis(axisY, Qt::AlignLeft);

        series->attachAxis(axisX);
        series->attachAxis(axisY);

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

    for (int i = 0; i < gridSizeX; i++)
    {
        for (int j = 0; j < gridSizeY; j++)
        {
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
    figure = 1;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Robinson et al.");
    ui->comboBox_func->addItem("Shi et al.");
    ui->comboBox_func->addItem("Po`ad et al.");
    ui->comboBox_func->addItem("Komnatnov M.E.");
    ui->comboBox_func->addItem("Nie et al.");

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

    double length = ui->lineEdit_size_a->text().toDouble();
    double width = ui->lineEdit_size_d->text().toDouble();
    double height = ui->lineEdit_size_b->text().toDouble();
    double notchDepth = ui->lineEdit_aperture_width->text().toDouble();
    double notchWidth = 0.002;                                                      // фиксированное значение ширины выреза
    double notchHeight = ui->lineEdit_aperture_height->text().toDouble();
    double xShift = ui->lineEdit_pos_x->text().toDouble();
    double yShift = ui->lineEdit_pos_y->text().toDouble();

    auto renderer = modelsFigure.createFigure1(
        length, width, height, notchDepth, notchWidth, notchHeight, xShift, yShift,
        ui->lineEdit_pos_x, ui->lineEdit_pos_y,
        ui->lineEdit_aperture_width, ui->lineEdit_aperture_height,
        ui->lineEdit_size_a, ui->lineEdit_size_b
        );

    error_occured = renderer.second;

    if(renderer.second)
        return;

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer.first);
    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}

void MainWindow::on_pushButton_Figure_2_clicked()
{
    figure = 2;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Ren et al.");
    ui->comboBox_func->addItem("Dehkhoda et al.");
    ui->comboBox_func->addItem("Nie et al.");

    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_pos_x->setText("Гор.");
    ui->label_pos_y->setText("Верт.");
    ui->label_pos->setText("Расстояние между центрами апертур");
    ui->label_aperture_height->setText("Высота");
    ui->label_aperture_width->show();
    ui->lineEdit_aperture_width->show();
    ui->lineEdit_pos_x->hide();
    ui->lineEdit_pos_y->hide();
    ui->lineEdit_col_horizontally->show();
    ui->lineEdit_col_vertically->show();
    ui->lineEdit_pos_horizontally->show();
    ui->lineEdit_pos_vertically->show();
    ui->label_horizontally->show();
    ui->label_vertically->show();
    ui->label_col->show();
    double width = ui->lineEdit_size_d->text().toDouble();
    double height = ui->lineEdit_size_b->text().toDouble();
    double length = ui->lineEdit_size_a->text().toDouble();

    double notchWidth = ui->lineEdit_aperture_width->text().toDouble();
    double notchHeight = ui->lineEdit_aperture_height->text().toDouble();
    double notchDepth = 0.003;

    int rows = ui->lineEdit_col_vertically->text().toInt();
    int cols = ui->lineEdit_col_horizontally->text().toInt();

    double horizontalSpacing = ui->lineEdit_pos_horizontally->text().toDouble();
    double verticalSpacing = ui->lineEdit_pos_vertically->text().toDouble();

    auto renderer = modelsFigure.createFigure2(width, height, length, notchWidth, notchHeight, notchDepth, rows, cols, horizontalSpacing, verticalSpacing,
    ui->lineEdit_aperture_height, ui->lineEdit_pos_vertically, ui->lineEdit_aperture_width,
    ui->lineEdit_pos_horizontally, ui->lineEdit_col_horizontally, ui->lineEdit_size_a, ui->lineEdit_col_vertically, ui->lineEdit_size_b,
    ui->lineEdit_aperture_width, ui->lineEdit_aperture_height,ui->lineEdit_size_a, ui->lineEdit_size_b);

    error_occured = renderer.second;

    if(renderer.second)
        return;

    renderer.first->SetBackground(colors->GetColor3d("White").GetData());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer.first);
    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}


void MainWindow::on_pushButton_Figure_3_clicked()
{
    figure = 3;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Wamg et al.");

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
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::red);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_size_b->setPalette(palette);
        error_occured = true;
        return;
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        ui->lineEdit_aperture_height->setPalette(palette);
        ui->lineEdit_size_b->setPalette(palette);
        error_occured = false;
    }

    vtkSmartPointer<vtkRenderer> renderer = modelsFigure.createFigure3(height, radius, notchHeight, notchRadius);

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();

}


void MainWindow::on_lineEdit_size_a_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
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


void MainWindow::on_lineEdit_size_b_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
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


void MainWindow::on_lineEdit_size_d_textChanged(const QString &arg1)
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



void MainWindow::on_pushButtonCalcStart_clicked()
{
    if (error_occured)
    {
        QMessageBox box;
        box.setText("Перед началом вычислений исправьте ошибку в параметрах");
        box.setWindowTitle("Error");
        box.exec();
        return;
    }

    double m_aVal = ui->lineEdit_size_a->text().toDouble();             // У цилиндра - h
    double m_bVal = ui->lineEdit_size_b->text().toDouble();             // У цилиндра - r
    double m_dVal = ui->lineEdit_size_d->text().toDouble();

    double m_apVal = ui->lineEdit_aperture_height->text().toDouble();   // У цилиндра - R апертуры

    double m_tVal = ui->lineEdit_figure_thickness->text().toDouble();   // Толщина

    double m_wVal = 0;
    double m_lVal = 0;
    double m_dvVal = 0;
    double m_dhVal = 0;
    int m_napVal = 0;
    int m_mapVal = 0;
    double m_xVal = 0;
    double m_yVal = 0;

    if (figure == 1)
    {
        m_wVal = ui->lineEdit_aperture_width->text().toDouble();         // Ширина апертуры
        m_lVal = ui->lineEdit_aperture_height->text().toDouble();        // Высота апертуры

        m_xVal = ui->lineEdit_pos_x->text().toDouble();                  // Расположение X в аппертуре
        m_yVal = ui->lineEdit_pos_y->text().toDouble();                  // Расположение Y в аппертуре
    }

    if (figure == 2)
    {
        m_wVal = ui->lineEdit_aperture_width->text().toDouble();         // Ширина апертуры
        m_lVal = ui->lineEdit_aperture_height->text().toDouble();        // Высота апертуры

        // Расстояние между центрами аппертур для 2-ой фигуры
        m_dvVal = ui->lineEdit_pos_vertically->text().toDouble();       // Гор
        m_dhVal = ui->lineEdit_pos_horizontally->text().toDouble();     // Вер

        // Количество аппертур для 2-ой фигуры
        m_napVal = ui->lineEdit_col_vertically->text().toInt();         // Гор
        m_mapVal = ui->lineEdit_col_horizontally->text().toInt();       // Вер
    }


    double m_fMinVal = ui->lineEdit_Source_Fmin->text().toDouble();         // F min
    double m_fMaxVal = ui->lineEdit_Source_Fmax->text().toDouble();         // F max

    double m_nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();       // Количество точек
    double m_integralVal = ui->lineEdit_POV_step->text().toInt();              // Шаг интегрирования
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();            // Количество точек наблюдения
    double m_pVal = ui->lineEdit_POV_P->text().toDouble();                  // P - точка наблюдения
    double m_sigmaVal = 37000000;                                           // Сигма

    double m_nVal = ui->lineEdit_Source_n->text().toInt();                     // n
    double m_mVal = ui->lineEdit_Source_m->text().toInt();                     // m

    bool m_RungeVal = ui->checkBox_Runge->isChecked();                        // Правило Рунге
    bool m_fileBool = ui->checkBox_File->isChecked();                         // Загрузка из файла

    calc_thread = new CalculationThread(m_fMinVal, m_fMaxVal, m_tVal, m_wVal, m_bVal, m_bVal, m_aVal, m_apVal, m_lVal, m_aVal,
                                  m_dVal, m_pVal, m_nPointsVal, m_pstepVal, m_xVal, m_yVal, m_napVal, m_mapVal, m_nVal,
                                  m_mVal, m_dvVal, m_dhVal, m_sigmaVal, m_integralVal, m_RungeVal, m_fileBool);

    int m_funcVal = 0;

    if(ui->comboBox_func->currentText() == "Robinson et al."){
        m_funcVal = 0;
    }
    else if(ui->comboBox_func->currentText() == "Shi et al."){
        m_funcVal = 1;
    }
    else if(ui->comboBox_func->currentText() == "Po`ad et al."){
        m_funcVal = 2;
    }
    else if(ui->comboBox_func->currentText() == "Komnatnov M.E."){
        m_funcVal = 3;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Wamg et al."){
        m_funcVal = 8;
    }

    calc_thread->mod = m_funcVal;

    connect(calc_thread, SIGNAL(progress(double)), this, SLOT(UpdateProgress(double)));
    //connect(calc_thread, SIGNAL(time(double)), this, SLOT(PrintCalcTime(double)));
    //connect(calc_thread, SIGNAL(iterCount(double)), this, SLOT(PrintCalcIter(double)));
    connect(calc_thread, SIGNAL(GUI(QVector<surfaceModelItem>)), this, SLOT(PrintGUI(QVector<surfaceModelItem>)));
    connect(calc_thread, SIGNAL(GUI(QVector<surfaceModelItem>)), this, SLOT(UpdateGUI(QVector<surfaceModelItem>)));

    calc_thread->start();
}

void MainWindow::CalcRAM()
{
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();
    double m_nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();

    double tmp = (((((100/m_pstepVal) * m_nPointsVal)*3*8)/1024)/1024 + (((100/m_pstepVal) * m_nPointsVal)*3)/1024 + 50);
    tmp = tmp + 0.5 - (tmp < 0);
    RAM = (int)tmp;

    ui->label_RAM_amount->setText(tr("%1 Мб.").arg(RAM));
}

void MainWindow::CalcTime()
{
    int m_funcVal = 0;

    if(ui->comboBox_func->currentText() == "Robinson et al."){
        m_funcVal = 0;
    }
    else if(ui->comboBox_func->currentText() == "Shi et al."){
        m_funcVal = 1;
    }
    else if(ui->comboBox_func->currentText() == "Po`ad et al."){
        m_funcVal = 2;
    }
    else if(ui->comboBox_func->currentText() == "Komnatnov M.E."){
        m_funcVal = 3;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Wamg et al."){
        m_funcVal = 8;
    }

    double m_nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();

    switch (m_funcVal) {
    case 0:
        Time = (m_pstepVal * m_nPointsVal) / 1000000;
        break;
    case 1:
        Time = (1000*(m_pstepVal) * m_nPointsVal) / 2000000;
        break;
    case 2:
        Time = (2*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 3:
        Time = (3*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 4:
        Time = (3*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 5:
        Time = ((m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 6:
        Time = (3*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 8:
        Time = (14*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    default:
        break;
    }

    ui->label_TIME_amount->setText(tr("%1 сек.").arg(Time));
}

void MainWindow::CalcDif()
{
    int m_funcVal = 0;

    if(ui->comboBox_func->currentText() == "Robinson et al."){
        m_funcVal = 0;
    }
    else if(ui->comboBox_func->currentText() == "Shi et al."){
        m_funcVal = 1;
    }
    else if(ui->comboBox_func->currentText() == "Po`ad et al."){
        m_funcVal = 2;
    }
    else if(ui->comboBox_func->currentText() == "Komnatnov M.E."){
        m_funcVal = 3;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Wamg et al."){
        m_funcVal = 8;
    }

    double m_nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();

    switch (m_funcVal) {
    case 0:
        Difficulty = (m_pstepVal * m_nPointsVal) / 10000000;
        break;
    case 1:
        Difficulty = (1000*(m_pstepVal) * m_nPointsVal) / 20000000;
        break;
    case 2:
        Difficulty = (2*(m_pstepVal) * m_nPointsVal) / 10000000;
        break;
    case 3:
        Difficulty = (3*(m_pstepVal) * m_nPointsVal) / 10000000;
        break;
    case 4:
        Difficulty = (3*(m_pstepVal) * m_nPointsVal) / 10000000;
        break;
    case 5:
        Difficulty = ((m_pstepVal) * m_nPointsVal) / 10000000;
        break;
    case 6:
        Difficulty = (3*(m_pstepVal) * m_nPointsVal) / 10000000;
        break;
    case 8:
        Difficulty = (14*(m_pstepVal) * m_nPointsVal) / 10000000 ;
        break;
    default:
        break;
    }

    ui->progressBar_diff->setValue(Difficulty);
}

void MainWindow::UpdateProgress(double val)
{
    if(val == 0)
    {
        ui->progressBar_calc->reset();
    }
    ui->progressBar_calc->setValue(val);
    //qDebug() <<val;
}

void MainWindow::PrintCalcTime(double val)
{
    //qDebug() <<"Time: " <<val <<"\n";
}

void MainWindow::PrintCalcIter(double val)
{
    //qDebug() <<"Iter: " <<val <<"\n";
}

void MainWindow::PrintGUI(QVector<surfaceModelItem> gui)
{
    for (int i = 0; i < gui.size(); i++)
    {
        //qDebug()<<"GUI" <<gui[i].x <<" " <<gui[i].y <<" " << gui[i].z;
    }
}

void MainWindow::UpdateGUI(QVector<surfaceModelItem> gui)
{
    mItems = gui;
}


void MainWindow::on_lineEdit_POV_NofPoints_textChanged(const QString &arg1)
{
    CalcRAM();
    CalcTime();
    CalcDif();
}


void MainWindow::on_lineEdit_Source_NofPoints_textChanged(const QString &arg1)
{
    CalcRAM();
    CalcTime();
    CalcDif();
}

void MainWindow::on_comboBox_func_currentTextChanged(const QString &arg1)
{
    CalcTime();
    CalcDif();
}

