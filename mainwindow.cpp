#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar_calc->setRange(0, 6666);
    for (int i = 0; i < ui->gridLayout_4->count(); ++i) {
        QLayoutItem *item = ui->gridLayout_4->itemAt(i);
        if (item && item->widget()) {
            ui->gridLayout_4->setAlignment(item->widget(), Qt::AlignLeft);
        }
    }
    for (int i = 0; i < ui->gridLayout_2->count(); ++i) {
        QLayoutItem *item = ui->gridLayout_2->itemAt(i);
        if (item && item->widget()) {
            ui->gridLayout_2->setAlignment(item->widget(), Qt::AlignLeft);
        }
    }
    ui->GRAPH_2D->hide();
    ui->pushButton_Save->hide();
    ui->pushButton_Reset->hide();
    on_pushButton_Figure_1_clicked();

    CalcRAM();
    CalcTime();
    CalcDif();

    hzf = new HerzFormatter();

    surface = new Q3DSurface();
    surface->axisX()->setFormatter(hzf);

    surface->axisZ()->setLabelFormat("%.5f");
    surface->axisY()->setLabelFormat("%d");
    surface->axisX()->setLabelFormat("%f");

    surface->axisX()->setTitle("Частота, Гц");
    surface->axisY()->setTitle("ЭЭ, дБ");
    surface->axisZ()->setTitle("Точка наблюдения, м");

    surface->axisX()->setTitleVisible(true);
    surface->axisY()->setTitleVisible(true);
    surface->axisZ()->setTitleVisible(true);

    surface->setHorizontalAspectRatio(0.8);

    surface->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);

    container = QWidget::createWindowContainer(surface, this);
    container->setMinimumSize(QSize(631, 400));
    QHBoxLayout *layout = new QHBoxLayout();
    ui->GRAPH_3D->setLayout(layout);
    layout->addWidget(container);
}


void MainWindow::PointSelected(const QPoint &position)
{
    if (position.x() < 0){
        return;
    }
    Create2DGraph(position.x());
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Create2DGraph(int num)
{
    series = new QLineSeries();

    QChart *chart = new QChart();

    SelectedPoint = num * calc_thread->m_nPointsVal;

    for (int i = 0; i < m_nPointsVal; ++i)
    {
        double x = mItems[SelectedPoint + i].x;
        double y = mItems[SelectedPoint + i].y;
        series->append(x, y);
    }

    series->setPen(QPen(QColor(46,144,250), 2, Qt::SolidLine));
    chart->addSeries(series);
    chart->setAnimationOptions(QChart::AllAnimations);

    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Частота, Гц");
    axisX->setTitleFont(QFont("OpenSans", 12, QFont::Bold));
    axisX->setLabelsFont(QFont("OpenSans", 10));
    axisX->setLabelFormat("%d");
    axisX->setTickCount(10);
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("ЭЭ, дБ");
    axisY->setTitleFont(QFont("OpenSans", 12, QFont::Bold));
    axisY->setLabelsFont(QFont("OpenSans", 10));
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
    if (!surface->seriesList().isEmpty()) {
        surface->removeSeries(series1);
        delete series1;
    }

    QSurfaceDataProxy *dataProxy = new QSurfaceDataProxy();
    series1 = new QSurface3DSeries(dataProxy);

    QSurfaceDataArray *dataArray = new QSurfaceDataArray();
    //const int gridSize = mItems.size(); // Размер сетки
    dataArray->reserve(calc_thread->m_pstepVal);

    int k = 0;

    for (int i = 0; i < calc_thread->m_pstepVal; ++i)
    {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(calc_thread->m_nPointsVal);
        for (int j = 0; j < calc_thread->m_nPointsVal; ++j)
        {
            double x = mItems[k].x;
            double y = mItems[k].y;
            double z = mItems[k].z;
            //qDebug()<<"x " << x <<"y " << y<<"z " << z;
            k++;

            (*newRow)[j].setPosition(QVector3D(x, y, z));
        }
        dataArray->append(newRow);
    }
    dataProxy->resetArray(dataArray);

    series1->setDrawMode(QSurface3DSeries::DrawSurface);
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::blue);
    gr.setColorAt(0.5, Qt::red);

    surface->addSeries(series1);

    surface->seriesList().at(0)->setBaseGradient(gr);
    surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    //double x1 = mItems[0].x - 0.5;
    //double x2 = mItems[(ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()) - 1].x + 0.5;

    //surface->axisX()->setRange(x1, x2);
    //surface->axisY()->setRange((float)mItems[0].y + 0.5, (float)mItems[ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()].y + 0.5);
    //surface->axisZ()->setRange(mItems[0].z - 0.5, mItems[(ui->lineEdit_POV_NofPoints->text().toInt() * ui->lineEdit_Source_NofPoints->text().toInt()) - 1].z + 0.5);

    connect(series1, SIGNAL(selectedPointChanged(const QPoint)), this, SLOT(PointSelected(const QPoint)));
    ui->pushButton_Reset->show();

}

void MainWindow::on_pushButton_2D_clicked()
{
    if(mItems.size() == 0)
    {
        QMessageBox box;
        box.setText("Перед построением графика необходимо\nвыполнить вычисления");
        box.setWindowTitle("Error");
        box.exec();
        return;
    }

    ui->pushButton_Save->show();
    ui->GRAPH_3D->hide();
    ui->GRAPH_2D->show();
}

void MainWindow::on_pushButton_3D_clicked()
{

    if(mItems.size() == 0)
    {
        QMessageBox box;
        box.setText("Перед построением графика необходимо\nвыполнить вычисления");
        box.setWindowTitle("Error");
        box.exec();
        return;
    }

    ui->pushButton_Save->hide();
    ui->GRAPH_3D->show();
    ui->GRAPH_2D->hide();
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

    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_size_a->setToolTip("X в декартовой системе координат");
    ui->label_size_b->setToolTip("Z в декартовой системе координат");
    ui->label_pos_x->setText("X"); 
    ui->label_pos_y->setText("Y");
    ui->label_pos->setText("Расположение");
    ui->label_aperture_height->setText("Высота");

    // ui->lineEdit_size_d->show();
    // ui->label_size_d->show();
    // ui->label_pos_x->show();
    // ui->label_pos_y->show();
    // ui->lineEdit_pos_x->show();
    // ui->lineEdit_pos_y->show();
    // ui->label_pos->show();
    // ui->label_aperture_width->show();
    // ui->lineEdit_aperture_width->show();

    ui->lineEdit_size_d->setEnabled(true);
    ui->label_size_d->setEnabled(true);
    ui->label_pos_x->setEnabled(true);
    ui->label_pos_y->setEnabled(true);
    ui->lineEdit_pos_x->setEnabled(true);
    ui->lineEdit_pos_y->setEnabled(true);
    ui->label_pos->setEnabled(true);
    ui->label_aperture_width->setEnabled(true);
    ui->lineEdit_aperture_width->setEnabled(true);

    // ui->lineEdit_col_horizontally->hide();
    // ui->lineEdit_col_vertically->hide();
    // ui->lineEdit_pos_horizontally->hide();
    // ui->lineEdit_pos_vertically->hide();

    /// ui->lineEdit_col_horizontally->setDisabled(true);
    /// ui->lineEdit_col_vertically->setDisabled(true);
    /// ui->lineEdit_pos_horizontally->setDisabled(true);
    /// ui->lineEdit_pos_vertically->setDisabled(true);

    /// ui->label_horizontally->setDisabled(true);
    /// ui->label_vertically->setDisabled(true);
    /// ui->label_col->setDisabled(true);

    ui->lineEdit_col_horizontally->setEnabled(false);
    ui->lineEdit_col_vertically->setEnabled(false);
    ui->lineEdit_pos_horizontally->setEnabled(false);
    ui->lineEdit_pos_vertically->setEnabled(false);

    ui->label_horizontally->setEnabled(false);
    ui->label_vertically->setEnabled(false);
    ui->label_col->setEnabled(false);

    create_figure_1();
}

void MainWindow::on_pushButton_Figure_2_clicked()
{
    figure = 2;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Ren et al.");
    ui->comboBox_func->addItem("Dehkhoda et al.");
    ui->comboBox_func->addItem("Nie et al.");

    ui->label_size_a->setText("a");
    ui->label_size_b->setText("b");
    ui->label_size_a->setToolTip("X в декартовой системе координат");
    ui->label_size_b->setToolTip("Z в декартовой системе координат");
    ui->label_pos_x->setText("Гор.");
    ui->label_pos_y->setText("Верт.");
    ui->label_pos->setText("Расстояние между центрами апертур");
    ui->label_aperture_height->setText("Высота");

    // ui->lineEdit_pos_x->hide();
    // ui->lineEdit_pos_y->hide();

    // ui->lineEdit_col_horizontally->show();
    // ui->lineEdit_col_vertically->show();
    // ui->lineEdit_pos_horizontally->show();
    // ui->lineEdit_pos_vertically->show();
    // ui->lineEdit_aperture_width->show();
    // ui->lineEdit_size_d->show();

    // ui->label_horizontally->show();
    // ui->label_vertically->show();
    // ui->label_col->show();
    // ui->label_aperture_width->show();
    // ui->label_size_d->show();

    ui->lineEdit_pos_x->setEnabled(false);
    ui->lineEdit_pos_y->setEnabled(false);

    ui->lineEdit_col_horizontally->setEnabled(true);
    ui->lineEdit_col_vertically->setEnabled(true);
    ui->lineEdit_pos_horizontally->setEnabled(true);
    ui->lineEdit_pos_vertically->setEnabled(true);
    ui->lineEdit_aperture_width->setEnabled(true);
    ui->lineEdit_size_d->setEnabled(true);

    ui->label_horizontally->setEnabled(true);
    ui->label_vertically->setEnabled(true);
    ui->label_col->setEnabled(true);
    ui->label_aperture_width->setEnabled(true);
    ui->label_size_d->setEnabled(true);


    create_figure_2();
}


void MainWindow::on_pushButton_Figure_3_clicked()
{
    figure = 3;

    ui->comboBox_func->clear();
    ui->comboBox_func->addItem("Wamg et al.");

    ui->label_size_a->setText("h");
    ui->label_size_a->setToolTip("Высота цилиндра");
    ui->label_size_b->setText("r");
    ui->label_size_b->setToolTip("Радиус цилиндра");
    ui->label_aperture_height->setText("Радиус");


    // ui->lineEdit_col_horizontally->hide();
    // ui->lineEdit_col_vertically->hide();
    // ui->lineEdit_pos_horizontally->hide();
    // ui->lineEdit_pos_vertically->hide();
    // ui->lineEdit_col_vertically->hide();
    // ui->lineEdit_col_horizontally->hide();
    // ui->lineEdit_pos_x->hide();
    // ui->lineEdit_pos_y->hide();
    // ui->lineEdit_size_d->hide();
    // ui->lineEdit_aperture_width->hide();

    // ui->label_col->hide();
    // ui->label_pos_x->hide();
    // ui->label_pos_y->hide();
    // ui->label_horizontally->hide();
    // ui->label_vertically->hide();
    // ui->label_pos->hide();
    // ui->label_aperture_width->hide();
    // ui->label_size_d->hide();

    ui->lineEdit_col_horizontally->setEnabled(false);
    ui->lineEdit_col_vertically->setEnabled(false);
    ui->lineEdit_pos_horizontally->setEnabled(false);
    ui->lineEdit_pos_vertically->setEnabled(false);
    ui->lineEdit_col_vertically->setEnabled(false);
    ui->lineEdit_col_horizontally->setEnabled(false);
    ui->lineEdit_pos_x->setEnabled(false);
    ui->lineEdit_pos_y->setEnabled(false);
    ui->lineEdit_size_d->setEnabled(false);
    ui->lineEdit_aperture_width->setEnabled(false);

    ui->label_col->setEnabled(false);
    ui->label_pos_x->setEnabled(false);
    ui->label_pos_y->setEnabled(false);
    ui->label_horizontally->setEnabled(false);
    ui->label_vertically->setEnabled(false);
    ui->label_pos->setEnabled(false);
    ui->label_aperture_width->setEnabled(false);
    ui->label_size_d->setEnabled(false);

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
        ui->lineEdit_size_a, ui->lineEdit_size_b, ui->lineEdit_POV_P->text().toDouble()
        );

    error_occured = renderer.second;

    if(renderer.second)
        return;

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer.first);
    renderWindow->SetAlphaBitPlanes(1);
    renderWindow->SetMultiSamples(0);
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
                                               ui->lineEdit_aperture_width, ui->lineEdit_aperture_height,ui->lineEdit_size_a, ui->lineEdit_size_b, ui->lineEdit_POV_P->text().toDouble());

    error_occured = renderer.second;

    if(renderer.second)
        return;



    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->SetAlphaBitPlanes(1);
    renderWindow->SetMultiSamples(0);
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

    vtkSmartPointer<vtkRenderer> renderer = modelsFigure.createFigure3(height, radius, notchHeight, notchRadius, ui->lineEdit_POV_P->text().toDouble());

    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);

    ui->qvtkWidget_3D_MODEL->setRenderWindow(renderWindow);
    ui->qvtkWidget_3D_MODEL->update();
}

void MainWindow::on_lineEdit_size_a_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    switch (figure)
    {
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

    switch (figure)
    {
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

    switch (figure)
    {
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

    switch (figure)
    {
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

    switch (figure)
    {
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
        m_dvVal = ui->lineEdit_pos_vertically->text().toDouble();        // Гор
        m_dhVal = ui->lineEdit_pos_horizontally->text().toDouble();      // Вер

        // Количество аппертур для 2-ой фигуры
        m_napVal = ui->lineEdit_col_vertically->text().toInt();          // Гор
        m_mapVal = ui->lineEdit_col_horizontally->text().toInt();        // Вер
    }


    double m_fMinVal = ui->lineEdit_Source_Fmin->text().toDouble();         // F min
    double m_fMaxVal = ui->lineEdit_Source_Fmax->text().toDouble();         // F max

    m_nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();        // Количество точек
    double m_integralVal = ui->lineEdit_POV_step->text().toInt();               // Шаг интегрирования
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();             // Количество точек наблюдения
    double m_pVal = ui->lineEdit_POV_P->text().toDouble();                      // P - точка наблюдения
    double m_sigmaVal = 37000000;                                               // Сигма

    double m_nVal = ui->lineEdit_Source_n->text().toInt();                     // n
    double m_mVal = ui->lineEdit_Source_m->text().toInt();                     // m

    bool m_RungeVal = ui->checkBox_Runge->isChecked();                        // Правило Рунге
    bool m_fileBool = ui->checkBox_File->isChecked();                         // Загрузка из файла

    double  perc_step = (m_dVal - m_pVal)/(m_pstepVal);

    calc_thread = new CalculationThread(m_fMinVal, m_fMaxVal, m_tVal, m_wVal, m_bVal, m_bVal, m_aVal, m_apVal, m_lVal, m_aVal,
                                  m_dVal, m_pVal, m_nPointsVal, m_xVal, m_yVal, m_napVal, m_mapVal, m_nVal,
                                  m_mVal, m_dvVal, m_dhVal, m_sigmaVal, m_integralVal, m_RungeVal, m_fileBool);

    int m_funcVal = 0;

    calc_thread->m_pstepVal = m_pstepVal;

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

    connect(calc_thread, SIGNAL(progress(double)), this, SLOT(UpdateProgress(double)));
    connect(calc_thread, SIGNAL(time(double)), this, SLOT(PrintCalcTime(double)));
    connect(calc_thread, SIGNAL(iterCount(double)), this, SLOT(PrintCalcIter(double)));
    connect(calc_thread, SIGNAL(GUI(QVector<surfaceModelItem>)), this, SLOT(UpdateGUI(QVector<surfaceModelItem>)));

    calc_thread->start();

}

void MainWindow::CalcRAM()
{
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();
    double nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();

    double tmp = (((((m_pstepVal / 100) * nPointsVal)*3*8)/1024)/1024 + (((m_pstepVal / 100) * nPointsVal)*3)/1024 + 50);
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

    double nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();

    switch (m_funcVal)
    {
    case 0:
        Time = (m_pstepVal * nPointsVal) / 1000000 * 3;
        break;
    case 1:
        Time = (1000*(m_pstepVal) * nPointsVal) / 2000000;
        break;
    case 2:
        Time = (2*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 3:
        Time = (3*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 4:
        Time = (3*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 5:
        Time = ((m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 6:
        Time = (3*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 7:
        Time = (10*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 8:
        Time = (14*(m_pstepVal) * nPointsVal) / 1000000;
        break;
    case 9:
        Time = (10*(m_pstepVal) * nPointsVal) / 1000000;
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

    double nPointsVal = ui->lineEdit_Source_NofPoints->text().toInt();
    double m_pstepVal = ui->lineEdit_POV_NofPoints->text().toInt();

    switch (m_funcVal) {
    case 0:
        Difficulty = (m_pstepVal * nPointsVal) / 10000000;
        break;
    case 1:
        Difficulty = (1000*(m_pstepVal) * nPointsVal) / 20000000;
        break;
    case 2:
        Difficulty = (2*(m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 3:
        Difficulty = (3*(m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 4:
        Difficulty = (3*(m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 5:
        Difficulty = ((m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 6:
        Difficulty = (3*(m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 7:
        Difficulty = (10*(m_pstepVal) * nPointsVal) / 10000000;
        break;
    case 8:
        Difficulty = (14*(m_pstepVal) * nPointsVal) / 10000000 ;
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
    ui->label_Calc_time_2->setText(tr("Время вычислений: %1 мс").arg(val));
}

void MainWindow::PrintCalcIter(double val)
{
    //qDebug() <<"Iter: " <<val <<"\n";
    ui->label_Iter_amount_2->setText(tr("Кол-во итераций: %1").arg(val));
}

void MainWindow::UpdateGUI(QVector<surfaceModelItem> gui)
{
    mItems = gui;
    Create3DGraph();
}


void MainWindow::on_lineEdit_POV_NofPoints_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    CalcRAM();
    CalcTime();
    CalcDif();
}


void MainWindow::on_lineEdit_Source_NofPoints_textChanged(const QString &arg1)
{
    if(arg1.toDouble() <= 0)
        return;

    CalcRAM();
    CalcTime();
    CalcDif();
}

void MainWindow::on_comboBox_func_currentTextChanged(const QString &arg1)
{  
    CalcTime();
    CalcDif();
}


void MainWindow::on_lineEdit_POV_P_textChanged(const QString &arg1)
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


void MainWindow::on_pushButton_Save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Data File", "", "Text Files (*.txt);;All Files (*)");

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "Частота, Гц\tЭЭ, дБ\n";
            out << mItems[SelectedPoint].x << "\t\t" << mItems[SelectedPoint].y << "\n";
            for (int i = 1; i < m_nPointsVal; ++i)
            {
                double x = mItems[SelectedPoint + i].x;
                double y = mItems[SelectedPoint + i].y;
                out << x << "\t" << y << "\n";
            }
            file.close();

            QMessageBox box;
            box.setText("Файл создан: " + filePath);
            box.exec();
        }
        else
        {
            QMessageBox box;
            box.setText("Не удалось открыть файл для записи");
            box.exec();
        }
    }
    else
    {
        QMessageBox box;
        box.setText("Файл не выбран");
        box.exec();
    }
}


void MainWindow::on_pushButton_Reset_clicked()
{
    if (!surface->seriesList().isEmpty()) {
        surface->removeSeries(series1);
        delete series1;
        mItems.clear();
        ui->pushButton_Reset->hide();
    }
}

