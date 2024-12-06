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
    //on_pushButton_2D_clicked();

    CalcRAM();
    CalcTime();
    CalcDif();

    surface = new Q3DSurface();
    surface->axisX()->setAutoAdjustRange(true);
    surface->axisY()->setAutoAdjustRange(true);
    surface->axisZ()->setAutoAdjustRange(true);
    surface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone); // Не работает :/ , должно отключать тени

    container = QWidget::createWindowContainer(surface, this);
    container->setMinimumSize(QSize(631, 400));
    QHBoxLayout *layout = new QHBoxLayout();
    ui->GRAPH_3D->setLayout(layout);
    layout->addWidget(container);
}


void MainWindow::PointSelected(const QPoint &position)
{
    //qDebug() <<position.x() <<" " <<position.y();
    if (position.x() < 0){
        return;
    }
    Create2DGraph(position.x());
    //on_pushButton_2D_clicked();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Create2DGraph(int num)
{

    series = new QLineSeries();

    QChart *chart = new QChart();

    int k = num * calc_thread->m_nPointsVal;

    for (int i = 0; i < ui->lineEdit_Source_NofPoints->text().toInt(); ++i)
    {
        double x = mItems[k + i].x /*/ 1000000000*/;
        double y = mItems[k + i].y /*/ 1000*/;
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
}

void MainWindow::Create3DGraph()
{

    QSurfaceDataProxy *dataProxy = new QSurfaceDataProxy();
    series1 = new QSurface3DSeries(dataProxy);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray();
    const int gridSize = mItems.size(); // Размер сетки
    dataArray->reserve(ui->lineEdit_POV_NofPoints->text().toInt());

    int k = 0;
    for (int i = 0; i < ui->lineEdit_POV_NofPoints->text().toInt(); ++i) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(gridSize);
        for (int j = 0; j < ui->lineEdit_Source_NofPoints->text().toInt(); ++j) {
            //qDebug()<< j<<"x " <<mItems[j].x  / 10000000000 <<"y " << mItems[j].y  / 1000<<"z " << mItems[j].z;
            double x = mItems[k].x / 1000000000;
            double y = mItems[k].y / 1000 ;
            double z = mItems[k].z;
            //qDebug()<< j<<"x " << x <<"y " << y<<"z " << z;
            k++;

            (*newRow)[j].setPosition(QVector3D(x, y, z));
        }
        dataArray->append(newRow);
    }
    dataProxy->resetArray(dataArray);

    // Настраиваем серию и оси
    //series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
    //surface->activeTheme()->setType(Q3DTheme::Theme(1));

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.33, Qt::blue);
    gr.setColorAt(0.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    surface->addSeries(series1);

    surface->seriesList().at(0)->setBaseGradient(gr);
    surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    //surface->axisX()->setRange((float)mItems[0].x + 0.5, (float)mItems[ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()].x + 0.5);
    //surface->axisY()->setRange((float)mItems[0].y + 0.5, (float)mItems[ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()].y + 0.5);
    //surface->axisZ()->setRange((float)mItems[0].z + 0.5, (float)mItems[ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()].z + 0.5);

    surface->axisX()->setTitle("X Axis");
    surface->axisY()->setTitle("Z Axis");
    surface->axisZ()->setTitle("Y Axis");

    connect(series1, SIGNAL(selectedPointChanged(const QPoint)), this, SLOT(PointSelected(const QPoint)));
}

void MainWindow::on_pushButton_2D_clicked()
{
    if(mItems.size() == 0){
        QMessageBox box;
        box.setText("Перед построением графика необходимо\nвыполнить вычисления");
        box.setWindowTitle("Error");
        box.exec();
        return;
    }

    ui->GRAPH_3D->hide();
    ui->GRAPH_2D->show();
}

void MainWindow::on_pushButton_3D_clicked()
{
    if(mItems.size() == 0){
        QMessageBox box;
        box.setText("Перед построением графика необходимо\nвыполнить вычисления");
        box.setWindowTitle("Error");
        box.exec();
        return;
    }
    ui->GRAPH_3D->show();
    ui->GRAPH_2D->hide();

    // Создаем Q3DSurface


    //container->setFocusPolicy(Qt::StrongFocus);
    //Create3DGraph();
}


void MainWindow::on_pushButton_Figure_1_clicked()
{
    figure = 1;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Robinson et al.");
    ui->comboBox_func->addItem("Shi et al.");
    ui->comboBox_func->addItem("Po`ad et al.");
    ui->comboBox_func->addItem("Komnatnov M.E.");
    ui->comboBox_func->addItem("Nie et al. (waveguide diagrams)");

    ui->lineEdit_size_d->show();
    ui->label_size_d->show();
    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_size_a->setToolTip("X в декартовой системе координат");
    ui->label_size_b->setToolTip("Z в декартовой системе координат");
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

    create_figure_1();
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
    ui->label_size_a->setToolTip("X в декартовой системе координат");
    ui->label_size_b->setToolTip("Z в декартовой системе координат");
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

    create_figure_2();
}


void MainWindow::on_pushButton_Figure_3_clicked()
{
    figure = 3;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Wamg et al.");

    ui->lineEdit_size_d->hide();
    ui->label_size_d->hide();
    ui->label_size_a->setText("h");
    ui->label_size_a->setToolTip("Высота цилиндра");
    ui->label_size_b->setText("r");
    ui->label_size_b->setToolTip("Радиус цилиндра");
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

    create_figure_3();
}


void MainWindow::create_figure_1()
{
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

void MainWindow::create_figure_2()
{
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

void MainWindow::create_figure_3()
{
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
        create_figure_3();
        break;
    case 2:
        create_figure_2();
        break;
    case 1:
        create_figure_1();
        break;
    }
}


void MainWindow::on_lineEdit_size_b_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
    case 3:
        create_figure_3();
        break;
    case 2:
        create_figure_2();
        break;
    case 1:
        create_figure_1();
        break;
    }
}


void MainWindow::on_lineEdit_size_d_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
    case 2:
        create_figure_2();
        break;
    case 1:
        create_figure_1();
        break;
    }
}


void MainWindow::on_lineEdit_pos_x_textChanged(const QString &arg1)
{
    create_figure_1();
}


void MainWindow::on_lineEdit_pos_y_textChanged(const QString &arg1)
{
    create_figure_1();
}


void MainWindow::on_lineEdit_aperture_height_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
    case 3:
        create_figure_3();
        break;
    case 2:
        create_figure_2();
        break;
    case 1:
        create_figure_1();
        break;
    }

}


void MainWindow::on_lineEdit_aperture_width_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure) {
    case 2:
        create_figure_2();
        break;
    case 1:
        create_figure_1();
        break;
    }

}


void MainWindow::on_lineEdit_col_horizontally_textChanged(const QString &arg1)//количество по горизонтале
{
    if(arg1.toDouble() <= 0)
        return;

    create_figure_2();
}


void MainWindow::on_lineEdit_col_vertically_textChanged(const QString &arg1)//количество по вертикале
{
    if(arg1.toDouble() <= 0)
        return;

    create_figure_2();
}


void MainWindow::on_lineEdit_pos_horizontally_textChanged(const QString &arg1)//количество по горизонтале
{
    if(arg1.toDouble() <= 0)
        return;

    create_figure_2();
}


void MainWindow::on_lineEdit_pos_vertically_textChanged(const QString &arg1)//растояние  по вертикале
{
    if(arg1.toDouble() <= 0)
        return;

    create_figure_2();
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

    double  perc_step;
    perc_step = (m_dVal - m_pVal)/(m_pstepVal);

    calc_thread = new CalculationThread(m_fMinVal, m_fMaxVal, m_tVal, m_wVal, m_bVal, m_bVal, m_aVal, m_apVal, m_lVal, m_aVal,
                                  m_dVal, m_pVal, m_nPointsVal, m_xVal, m_yVal, m_napVal, m_mapVal, m_nVal,
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
    else if(ui->comboBox_func->currentText() == "Nie et al. (waveguide diagrams)"){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 7;
    }
    else if(ui->comboBox_func->currentText() == "Wamg et al."){
        m_funcVal = 8;
    }

    calc_thread->mod = m_funcVal;
    calc_thread->perc_step = perc_step;
    calc_thread->modS = ui->comboBox_herz->currentIndex();          // Для загрузки из файла

    // qDebug()<<"F min " <<calc_thread->m_fMinVal;
    // qDebug()<<"F max " <<calc_thread->m_fMaxVal;
    // qDebug()<<"m_tVal " <<calc_thread->m_tVal;
    // qDebug()<<"m_wVal " <<calc_thread->m_wVal;
    // qDebug()<<"m_bVal " <<calc_thread->m_bVal;
    // qDebug()<<"m_rVal " <<calc_thread->m_bVal;
    // qDebug()<<"m_hVal " <<calc_thread->m_aVal;
    // qDebug()<<"m_apVa " <<calc_thread->m_apVal;
    // qDebug()<<"m_lVal " <<calc_thread->m_lVal;
    // qDebug()<<"m_aVal " <<calc_thread->m_aVal;
    // qDebug()<<"m_dVal " <<calc_thread->m_dVal;
    // qDebug()<<"m_pVal " <<calc_thread->m_pVal;
    // qDebug()<<"m_nPointsVal " <<calc_thread->m_nPointsVal;
    // qDebug()<<"m_pstepVal " <<calc_thread->m_perc_step;
    // qDebug()<<"m_xVal " <<calc_thread->m_xVal;
    // qDebug()<<"m_yVal " <<calc_thread->m_yVal;
    // qDebug()<<"m_napVal " <<calc_thread->m_napVal;
    // qDebug()<<"m_mapVal " <<calc_thread->m_mapVal;
    // qDebug()<<"m_nVal " <<calc_thread->m_nVal;
    // qDebug()<<"m_mVal " <<calc_thread->m_mVal;
    // qDebug()<<"m_dvVal " <<calc_thread->m_dvVal;
    // qDebug()<<"m_dhVal " <<calc_thread->m_dhVal;
    // qDebug()<<"m_sigmaVal " <<calc_thread->m_sigmaVal;
    // qDebug()<<"m_integralVal " <<calc_thread->m_integralVal;
    // qDebug()<<"m_RungeVal " <<calc_thread->m_RungeVal;
    // qDebug()<<"m_fileBool " <<calc_thread->m_fileBool;
    // qDebug()<<"mod " <<calc_thread->mod;
    // qDebug()<<"modS " <<calc_thread->modS;

    connect(calc_thread, SIGNAL(progress(double)), this, SLOT(UpdateProgress(double)));
    connect(calc_thread, SIGNAL(time(double)), this, SLOT(PrintCalcTime(double)));
    connect(calc_thread, SIGNAL(iterCount(double)), this, SLOT(PrintCalcIter(double)));
    //connect(calc_thread, SIGNAL(GUI(QVector<surfaceModelItem>)), this, SLOT(PrintGUI(QVector<surfaceModelItem>)));
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

    ui->label_RAM_amount->setText(tr("%1 МБ").arg(RAM));
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
    else if(ui->comboBox_func->currentText() == "Nie et al. (waveguide diagrams)"){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 7;
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
    case 7:
        Time = (10*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 8:
        Time = (14*(m_pstepVal) * m_nPointsVal) / 1000000;
        break;
    case 9:
        Time = (10*(m_pstepVal) * m_nPointsVal) / 1000000;
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
    else if(ui->comboBox_func->currentText() == "Nie et al. (waveguide diagrams)"){
        m_funcVal = 4;
    }
    else if(ui->comboBox_func->currentText() == "Ren et al."){
        m_funcVal = 5;
    }
    else if(ui->comboBox_func->currentText() == "Dehkhoda et al."){
        m_funcVal = 6;
    }
    else if(ui->comboBox_func->currentText() == "Nie et al."){
        m_funcVal = 7;
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
    case 7:
        Difficulty = (10*(m_pstepVal) * m_nPointsVal) / 10000000;
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
    ui->label_Calc_time->setText(tr("Время вычислений: %1 мс").arg(val));
}

void MainWindow::PrintCalcIter(double val)
{
    //qDebug() <<"Iter: " <<val <<"\n";
    ui->label_Iter_amount->setText(tr("Кол-во итераций: %1").arg(val));
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
    Create3DGraph();
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

