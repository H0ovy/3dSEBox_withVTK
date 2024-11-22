/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qvtkopenglnativewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVTKOpenGLNativeWidget *qvtkWidget_3D_MODEL;
    QLabel *label_3DSEBOX;
    QWidget *layoutWidget;
    QGridLayout *LayoutModeling;
    QLabel *label_aperture_height;
    QLabel *label_horizontally;
    QLineEdit *lineEdit_pos_horizontally;
    QLabel *label_size_b;
    QLabel *label_2;
    QLabel *label_col;
    QLineEdit *lineEdit_pos_y;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_col_horizontally;
    QLabel *label_size_d;
    QLabel *label_pos_y;
    QLabel *label_pos;
    QLabel *label_vertically;
    QLineEdit *lineEdit_pos_vertically;
    QLabel *label_3;
    QLabel *label_pos_x;
    QLabel *label_aperture_width;
    QLineEdit *lineEdit_size_d;
    QLineEdit *lineEdit_size_b;
    QLineEdit *lineEdit_pos_x;
    QLabel *label_size_a;
    QLineEdit *lineEdit_size_a;
    QLineEdit *lineEdit_aperture_width;
    QLineEdit *lineEdit_col_vertically;
    QLineEdit *lineEdit_aperture_height;
    QLabel *label_aperture;
    QLabel *label;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLabel *label_POV;
    QLabel *label_Source_Fmin;
    QLabel *label_Source_Fmax;
    QLabel *label_Source_NofPoints;
    QLabel *label_POV_P;
    QLabel *label_POV_NofPoints;
    QLineEdit *lineEdit_Source_Fmin;
    QLineEdit *lineEdit_Source_Fmax;
    QLineEdit *lineEdit_Source_NofPoints;
    QLineEdit *lineEdit_POV_P;
    QLineEdit *lineEdit_POV_NofPoints;
    QLabel *label_Source_m;
    QLabel *label_POV_func;
    QComboBox *comboBox_func;
    QLabel *label_POV_step;
    QComboBox *comboBox_herz;
    QCheckBox *checkBox;
    QLineEdit *lineEdit_POV_step;
    QCheckBox *checkBox_Runge;
    QLineEdit *lineEdit_Source_m;
    QLineEdit *lineEdit_Source_n;
    QLabel *label_Source;
    QLabel *label_Source_n;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_calc_diff;
    QProgressBar *progressBar_calc;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_TIME;
    QLabel *label_TIME_amount;
    QLabel *label_RAM;
    QLabel *label_RAM_amount;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_2D;
    QPushButton *pushButton_3D;
    QChartView *GRAPH_2D;
    QVTKOpenGLNativeWidget *qvtkWidget_GRAPH;
    QWidget *layoutWidget5;
    QGridLayout *gridLayout;
    QPushButton *pushButton_Figure_2;
    QPushButton *pushButton_Figure_1;
    QPushButton *pushButton_Figure_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1039, 848);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        qvtkWidget_3D_MODEL = new QVTKOpenGLNativeWidget(centralwidget);
        qvtkWidget_3D_MODEL->setObjectName("qvtkWidget_3D_MODEL");
        qvtkWidget_3D_MODEL->setGeometry(QRect(610, 520, 411, 311));
        label_3DSEBOX = new QLabel(centralwidget);
        label_3DSEBOX->setObjectName("label_3DSEBOX");
        label_3DSEBOX->setGeometry(QRect(20, 20, 231, 51));
        QFont font;
        font.setPointSize(32);
        label_3DSEBOX->setFont(font);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(690, 120, 311, 396));
        LayoutModeling = new QGridLayout(layoutWidget);
        LayoutModeling->setObjectName("LayoutModeling");
        LayoutModeling->setContentsMargins(0, 0, 0, 0);
        label_aperture_height = new QLabel(layoutWidget);
        label_aperture_height->setObjectName("label_aperture_height");

        LayoutModeling->addWidget(label_aperture_height, 7, 0, 1, 1);

        label_horizontally = new QLabel(layoutWidget);
        label_horizontally->setObjectName("label_horizontally");

        LayoutModeling->addWidget(label_horizontally, 10, 0, 1, 1);

        lineEdit_pos_horizontally = new QLineEdit(layoutWidget);
        lineEdit_pos_horizontally->setObjectName("lineEdit_pos_horizontally");

        LayoutModeling->addWidget(lineEdit_pos_horizontally, 11, 0, 1, 1);

        label_size_b = new QLabel(layoutWidget);
        label_size_b->setObjectName("label_size_b");

        LayoutModeling->addWidget(label_size_b, 2, 1, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        LayoutModeling->addWidget(label_2, 1, 0, 1, 4);

        label_col = new QLabel(layoutWidget);
        label_col->setObjectName("label_col");

        LayoutModeling->addWidget(label_col, 12, 0, 1, 3);

        lineEdit_pos_y = new QLineEdit(layoutWidget);
        lineEdit_pos_y->setObjectName("lineEdit_pos_y");

        LayoutModeling->addWidget(lineEdit_pos_y, 14, 1, 1, 2);

        lineEdit = new QLineEdit(layoutWidget);
        lineEdit->setObjectName("lineEdit");

        LayoutModeling->addWidget(lineEdit, 5, 0, 1, 3);

        lineEdit_col_horizontally = new QLineEdit(layoutWidget);
        lineEdit_col_horizontally->setObjectName("lineEdit_col_horizontally");

        LayoutModeling->addWidget(lineEdit_col_horizontally, 15, 0, 1, 1);

        label_size_d = new QLabel(layoutWidget);
        label_size_d->setObjectName("label_size_d");

        LayoutModeling->addWidget(label_size_d, 2, 3, 1, 1);

        label_pos_y = new QLabel(layoutWidget);
        label_pos_y->setObjectName("label_pos_y");

        LayoutModeling->addWidget(label_pos_y, 13, 1, 1, 2);

        label_pos = new QLabel(layoutWidget);
        label_pos->setObjectName("label_pos");

        LayoutModeling->addWidget(label_pos, 9, 0, 1, 3);

        label_vertically = new QLabel(layoutWidget);
        label_vertically->setObjectName("label_vertically");

        LayoutModeling->addWidget(label_vertically, 10, 1, 1, 1);

        lineEdit_pos_vertically = new QLineEdit(layoutWidget);
        lineEdit_pos_vertically->setObjectName("lineEdit_pos_vertically");

        LayoutModeling->addWidget(lineEdit_pos_vertically, 11, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");

        LayoutModeling->addWidget(label_3, 4, 0, 1, 2);

        label_pos_x = new QLabel(layoutWidget);
        label_pos_x->setObjectName("label_pos_x");

        LayoutModeling->addWidget(label_pos_x, 13, 0, 1, 1);

        label_aperture_width = new QLabel(layoutWidget);
        label_aperture_width->setObjectName("label_aperture_width");

        LayoutModeling->addWidget(label_aperture_width, 7, 1, 1, 2);

        lineEdit_size_d = new QLineEdit(layoutWidget);
        lineEdit_size_d->setObjectName("lineEdit_size_d");

        LayoutModeling->addWidget(lineEdit_size_d, 3, 3, 1, 2);

        lineEdit_size_b = new QLineEdit(layoutWidget);
        lineEdit_size_b->setObjectName("lineEdit_size_b");

        LayoutModeling->addWidget(lineEdit_size_b, 3, 1, 1, 2);

        lineEdit_pos_x = new QLineEdit(layoutWidget);
        lineEdit_pos_x->setObjectName("lineEdit_pos_x");

        LayoutModeling->addWidget(lineEdit_pos_x, 14, 0, 1, 1);

        label_size_a = new QLabel(layoutWidget);
        label_size_a->setObjectName("label_size_a");

        LayoutModeling->addWidget(label_size_a, 2, 0, 1, 1);

        lineEdit_size_a = new QLineEdit(layoutWidget);
        lineEdit_size_a->setObjectName("lineEdit_size_a");

        LayoutModeling->addWidget(lineEdit_size_a, 3, 0, 1, 1);

        lineEdit_aperture_width = new QLineEdit(layoutWidget);
        lineEdit_aperture_width->setObjectName("lineEdit_aperture_width");

        LayoutModeling->addWidget(lineEdit_aperture_width, 8, 1, 1, 2);

        lineEdit_col_vertically = new QLineEdit(layoutWidget);
        lineEdit_col_vertically->setObjectName("lineEdit_col_vertically");

        LayoutModeling->addWidget(lineEdit_col_vertically, 15, 1, 1, 1);

        lineEdit_aperture_height = new QLineEdit(layoutWidget);
        lineEdit_aperture_height->setObjectName("lineEdit_aperture_height");

        LayoutModeling->addWidget(lineEdit_aperture_height, 8, 0, 1, 1);

        label_aperture = new QLabel(layoutWidget);
        label_aperture->setObjectName("label_aperture");

        LayoutModeling->addWidget(label_aperture, 6, 0, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        LayoutModeling->addWidget(label, 0, 0, 1, 4);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(60, 580, 491, 181));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_POV = new QLabel(layoutWidget1);
        label_POV->setObjectName("label_POV");

        gridLayout_2->addWidget(label_POV, 0, 4, 1, 3);

        label_Source_Fmin = new QLabel(layoutWidget1);
        label_Source_Fmin->setObjectName("label_Source_Fmin");

        gridLayout_2->addWidget(label_Source_Fmin, 1, 0, 1, 1);

        label_Source_Fmax = new QLabel(layoutWidget1);
        label_Source_Fmax->setObjectName("label_Source_Fmax");

        gridLayout_2->addWidget(label_Source_Fmax, 1, 1, 1, 2);

        label_Source_NofPoints = new QLabel(layoutWidget1);
        label_Source_NofPoints->setObjectName("label_Source_NofPoints");

        gridLayout_2->addWidget(label_Source_NofPoints, 1, 3, 1, 1);

        label_POV_P = new QLabel(layoutWidget1);
        label_POV_P->setObjectName("label_POV_P");

        gridLayout_2->addWidget(label_POV_P, 1, 4, 1, 2);

        label_POV_NofPoints = new QLabel(layoutWidget1);
        label_POV_NofPoints->setObjectName("label_POV_NofPoints");

        gridLayout_2->addWidget(label_POV_NofPoints, 1, 6, 1, 1);

        lineEdit_Source_Fmin = new QLineEdit(layoutWidget1);
        lineEdit_Source_Fmin->setObjectName("lineEdit_Source_Fmin");

        gridLayout_2->addWidget(lineEdit_Source_Fmin, 2, 0, 1, 1);

        lineEdit_Source_Fmax = new QLineEdit(layoutWidget1);
        lineEdit_Source_Fmax->setObjectName("lineEdit_Source_Fmax");

        gridLayout_2->addWidget(lineEdit_Source_Fmax, 2, 1, 1, 2);

        lineEdit_Source_NofPoints = new QLineEdit(layoutWidget1);
        lineEdit_Source_NofPoints->setObjectName("lineEdit_Source_NofPoints");

        gridLayout_2->addWidget(lineEdit_Source_NofPoints, 2, 3, 1, 1);

        lineEdit_POV_P = new QLineEdit(layoutWidget1);
        lineEdit_POV_P->setObjectName("lineEdit_POV_P");

        gridLayout_2->addWidget(lineEdit_POV_P, 2, 4, 1, 1);

        lineEdit_POV_NofPoints = new QLineEdit(layoutWidget1);
        lineEdit_POV_NofPoints->setObjectName("lineEdit_POV_NofPoints");

        gridLayout_2->addWidget(lineEdit_POV_NofPoints, 2, 6, 1, 1);

        label_Source_m = new QLabel(layoutWidget1);
        label_Source_m->setObjectName("label_Source_m");

        gridLayout_2->addWidget(label_Source_m, 3, 2, 2, 2);

        label_POV_func = new QLabel(layoutWidget1);
        label_POV_func->setObjectName("label_POV_func");

        gridLayout_2->addWidget(label_POV_func, 3, 4, 1, 2);

        comboBox_func = new QComboBox(layoutWidget1);
        comboBox_func->addItem(QString());
        comboBox_func->addItem(QString());
        comboBox_func->addItem(QString());
        comboBox_func->addItem(QString());
        comboBox_func->addItem(QString());
        comboBox_func->setObjectName("comboBox_func");

        gridLayout_2->addWidget(comboBox_func, 4, 4, 2, 3);

        label_POV_step = new QLabel(layoutWidget1);
        label_POV_step->setObjectName("label_POV_step");

        gridLayout_2->addWidget(label_POV_step, 6, 4, 1, 3);

        comboBox_herz = new QComboBox(layoutWidget1);
        comboBox_herz->addItem(QString());
        comboBox_herz->addItem(QString());
        comboBox_herz->addItem(QString());
        comboBox_herz->setObjectName("comboBox_herz");

        gridLayout_2->addWidget(comboBox_herz, 7, 0, 1, 2);

        checkBox = new QCheckBox(layoutWidget1);
        checkBox->setObjectName("checkBox");

        gridLayout_2->addWidget(checkBox, 7, 2, 1, 2);

        lineEdit_POV_step = new QLineEdit(layoutWidget1);
        lineEdit_POV_step->setObjectName("lineEdit_POV_step");

        gridLayout_2->addWidget(lineEdit_POV_step, 7, 4, 1, 1);

        checkBox_Runge = new QCheckBox(layoutWidget1);
        checkBox_Runge->setObjectName("checkBox_Runge");

        gridLayout_2->addWidget(checkBox_Runge, 7, 5, 1, 2);

        lineEdit_Source_m = new QLineEdit(layoutWidget1);
        lineEdit_Source_m->setObjectName("lineEdit_Source_m");

        gridLayout_2->addWidget(lineEdit_Source_m, 5, 2, 2, 2);

        lineEdit_Source_n = new QLineEdit(layoutWidget1);
        lineEdit_Source_n->setObjectName("lineEdit_Source_n");

        gridLayout_2->addWidget(lineEdit_Source_n, 5, 0, 2, 2);

        label_Source = new QLabel(layoutWidget1);
        label_Source->setObjectName("label_Source");

        gridLayout_2->addWidget(label_Source, 0, 0, 1, 3);

        label_Source_n = new QLabel(layoutWidget1);
        label_Source_n->setObjectName("label_Source_n");

        gridLayout_2->addWidget(label_Source_n, 3, 0, 2, 2);

        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName("layoutWidget2");
        layoutWidget2->setGeometry(QRect(730, 10, 231, 31));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_calc_diff = new QLabel(layoutWidget2);
        label_calc_diff->setObjectName("label_calc_diff");

        horizontalLayout_2->addWidget(label_calc_diff);

        progressBar_calc = new QProgressBar(layoutWidget2);
        progressBar_calc->setObjectName("progressBar_calc");
        progressBar_calc->setValue(56);

        horizontalLayout_2->addWidget(progressBar_calc);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName("layoutWidget3");
        layoutWidget3->setGeometry(QRect(410, 10, 251, 31));
        horizontalLayout = new QHBoxLayout(layoutWidget3);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_TIME = new QLabel(layoutWidget3);
        label_TIME->setObjectName("label_TIME");
        QFont font1;
        font1.setPointSize(16);
        label_TIME->setFont(font1);

        horizontalLayout->addWidget(label_TIME);

        label_TIME_amount = new QLabel(layoutWidget3);
        label_TIME_amount->setObjectName("label_TIME_amount");

        horizontalLayout->addWidget(label_TIME_amount);

        label_RAM = new QLabel(layoutWidget3);
        label_RAM->setObjectName("label_RAM");
        label_RAM->setFont(font1);

        horizontalLayout->addWidget(label_RAM);

        label_RAM_amount = new QLabel(layoutWidget3);
        label_RAM_amount->setObjectName("label_RAM_amount");

        horizontalLayout->addWidget(label_RAM_amount);

        layoutWidget4 = new QWidget(centralwidget);
        layoutWidget4->setObjectName("layoutWidget4");
        layoutWidget4->setGeometry(QRect(20, 80, 168, 26));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        pushButton_2D = new QPushButton(layoutWidget4);
        pushButton_2D->setObjectName("pushButton_2D");

        horizontalLayout_3->addWidget(pushButton_2D);

        pushButton_3D = new QPushButton(layoutWidget4);
        pushButton_3D->setObjectName("pushButton_3D");

        horizontalLayout_3->addWidget(pushButton_3D);

        GRAPH_2D = new QChartView(centralwidget);
        GRAPH_2D->setObjectName("GRAPH_2D");
        GRAPH_2D->setGeometry(QRect(30, 120, 451, 301));
        qvtkWidget_GRAPH = new QVTKOpenGLNativeWidget(centralwidget);
        qvtkWidget_GRAPH->setObjectName("qvtkWidget_GRAPH");
        qvtkWidget_GRAPH->setGeometry(QRect(10, 120, 661, 391));
        layoutWidget5 = new QWidget(centralwidget);
        layoutWidget5->setObjectName("layoutWidget5");
        layoutWidget5->setGeometry(QRect(690, 60, 311, 51));
        gridLayout = new QGridLayout(layoutWidget5);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_Figure_2 = new QPushButton(layoutWidget5);
        pushButton_Figure_2->setObjectName("pushButton_Figure_2");

        gridLayout->addWidget(pushButton_Figure_2, 0, 1, 1, 1);

        pushButton_Figure_1 = new QPushButton(layoutWidget5);
        pushButton_Figure_1->setObjectName("pushButton_Figure_1");

        gridLayout->addWidget(pushButton_Figure_1, 0, 0, 1, 1);

        pushButton_Figure_3 = new QPushButton(layoutWidget5);
        pushButton_Figure_3->setObjectName("pushButton_Figure_3");

        gridLayout->addWidget(pushButton_Figure_3, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_3DSEBOX->setText(QCoreApplication::translate("MainWindow", "3D SE BOX", nullptr));
        label_aperture_height->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\201\320\276\321\202\320\260", nullptr));
        label_horizontally->setText(QCoreApplication::translate("MainWindow", "\320\223\320\276\321\200.", nullptr));
        lineEdit_pos_horizontally->setText(QCoreApplication::translate("MainWindow", "0.02", nullptr));
        label_size_b->setText(QCoreApplication::translate("MainWindow", "b", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\321\200\320\277\321\203\321\201 (\320\262 \320\274.)", nullptr));
        label_col->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\260\320\277\320\265\321\200\321\202\321\203\321\200", nullptr));
        lineEdit_pos_y->setText(QCoreApplication::translate("MainWindow", "0.06", nullptr));
        lineEdit->setText(QCoreApplication::translate("MainWindow", "0.0015", nullptr));
        lineEdit_col_horizontally->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        label_size_d->setText(QCoreApplication::translate("MainWindow", "d", nullptr));
        label_pos_y->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_pos->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\321\201\320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265", nullptr));
        label_vertically->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\321\200\321\202.", nullptr));
        lineEdit_pos_vertically->setText(QCoreApplication::translate("MainWindow", "0.02", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\242\320\276\320\273\321\211\320\270\320\275\320\260", nullptr));
        label_pos_x->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_aperture_width->setText(QCoreApplication::translate("MainWindow", "\320\250\320\270\321\200\320\270\320\275\320\260", nullptr));
        lineEdit_size_d->setText(QCoreApplication::translate("MainWindow", "0.3", nullptr));
        lineEdit_size_b->setText(QCoreApplication::translate("MainWindow", "0.12", nullptr));
        lineEdit_pos_x->setText(QCoreApplication::translate("MainWindow", "0.15", nullptr));
        label_size_a->setText(QCoreApplication::translate("MainWindow", "a", nullptr));
        lineEdit_size_a->setText(QCoreApplication::translate("MainWindow", "0.3", nullptr));
        lineEdit_aperture_width->setText(QCoreApplication::translate("MainWindow", "0.012", nullptr));
        lineEdit_col_vertically->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        lineEdit_aperture_height->setText(QCoreApplication::translate("MainWindow", "0.012", nullptr));
        label_aperture->setText(QCoreApplication::translate("MainWindow", "\320\220\320\277\320\265\321\200\321\202\321\203\321\200\320\260", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\264\320\265\320\273\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265", nullptr));
        label_POV->setText(QCoreApplication::translate("MainWindow", "\320\242\320\276\321\207\320\272\320\260 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\321\217", nullptr));
        label_Source_Fmin->setText(QCoreApplication::translate("MainWindow", "F min", nullptr));
        label_Source_Fmax->setText(QCoreApplication::translate("MainWindow", "F max", nullptr));
        label_Source_NofPoints->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \321\202\320\276\321\207\320\265\320\272", nullptr));
        label_POV_P->setText(QCoreApplication::translate("MainWindow", "P", nullptr));
        label_POV_NofPoints->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276 \321\202\320\276\321\207\320\265\320\272 \320\275\320\260\320\261\320\273\321\216\320\264\320\265\320\275\320\270\321\217", nullptr));
        lineEdit_Source_Fmin->setText(QCoreApplication::translate("MainWindow", "1e+06", nullptr));
        lineEdit_Source_Fmax->setText(QCoreApplication::translate("MainWindow", "1e+09", nullptr));
        lineEdit_Source_NofPoints->setText(QCoreApplication::translate("MainWindow", "1000", nullptr));
        lineEdit_POV_P->setText(QCoreApplication::translate("MainWindow", "0.15", nullptr));
        lineEdit_POV_NofPoints->setText(QCoreApplication::translate("MainWindow", "10", nullptr));
        label_Source_m->setText(QCoreApplication::translate("MainWindow", "m", nullptr));
        label_POV_func->setText(QCoreApplication::translate("MainWindow", "\320\244\321\203\320\275\320\272\321\206\320\270\321\217", nullptr));
        comboBox_func->setItemText(0, QCoreApplication::translate("MainWindow", "Robinson et al.", nullptr));
        comboBox_func->setItemText(1, QCoreApplication::translate("MainWindow", "Shi et al.", nullptr));
        comboBox_func->setItemText(2, QCoreApplication::translate("MainWindow", "Po`ad et al.", nullptr));
        comboBox_func->setItemText(3, QCoreApplication::translate("MainWindow", "Komnatnov M.E.", nullptr));
        comboBox_func->setItemText(4, QCoreApplication::translate("MainWindow", "Nie et al. (waveguide diagrams)", nullptr));

        label_POV_step->setText(QCoreApplication::translate("MainWindow", "\320\250\320\260\320\263 \320\270\320\275\321\202\320\265\320\263\321\200\320\270\321\200\320\276\320\262\320\260\320\275\320\270\321\217", nullptr));
        comboBox_herz->setItemText(0, QCoreApplication::translate("MainWindow", "\320\223\321\206", nullptr));
        comboBox_herz->setItemText(1, QCoreApplication::translate("MainWindow", "\320\234\320\223\321\206", nullptr));
        comboBox_herz->setItemText(2, QCoreApplication::translate("MainWindow", "\320\223\320\223\321\206", nullptr));

        checkBox->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\272\320\260 \320\270\320\267 \321\204\320\260\320\271\320\273\320\260", nullptr));
        lineEdit_POV_step->setText(QCoreApplication::translate("MainWindow", "100", nullptr));
        checkBox_Runge->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\260\320\262\320\270\320\273\320\276 \320\240\321\203\320\275\320\263\320\265", nullptr));
        lineEdit_Source_m->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        lineEdit_Source_n->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_Source->setText(QCoreApplication::translate("MainWindow", "\320\230\321\201\321\202\320\276\321\207\320\275\320\270\320\272", nullptr));
        label_Source_n->setText(QCoreApplication::translate("MainWindow", "n", nullptr));
        label_calc_diff->setText(QCoreApplication::translate("MainWindow", "\320\241\320\273\320\276\320\266\320\275\320\276\321\201\321\202\321\214 \320\262\321\213\321\207.", nullptr));
        label_TIME->setText(QCoreApplication::translate("MainWindow", "\342\217\260", nullptr));
        label_TIME_amount->setText(QCoreApplication::translate("MainWindow", "0.01 \321\201\320\265\320\272.", nullptr));
        label_RAM->setText(QCoreApplication::translate("MainWindow", "\360\237\220\217", nullptr));
        label_RAM_amount->setText(QCoreApplication::translate("MainWindow", "800 \320\234\320\261.", nullptr));
        pushButton_2D->setText(QCoreApplication::translate("MainWindow", "2D", nullptr));
        pushButton_3D->setText(QCoreApplication::translate("MainWindow", "3D", nullptr));
        pushButton_Figure_2->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\321\217\320\274\320\276\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272 2", nullptr));
        pushButton_Figure_1->setText(QCoreApplication::translate("MainWindow", "\320\237\321\200\321\217\320\274\320\276\321\203\320\263\320\276\320\273\321\214\320\275\320\270\320\272 1", nullptr));
        pushButton_Figure_3->setText(QCoreApplication::translate("MainWindow", "\320\246\320\270\320\273\320\270\320\275\320\264\321\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
