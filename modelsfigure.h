#ifndef MODELSFIGURE_H
#define MODELSFIGURE_H

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkAppendPolyData.h>
#include <vtkProperty.h>
#include <vtkNamedColors.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkTextActor3D.h>
#include <vtkTextProperty.h>
#include <QLineEdit>
#include <QPalette>

#include <vtkVectorText.h>

class ModelsFigure
{
public:
    ModelsFigure();



    //
    std::pair<vtkSmartPointer<vtkRenderer>, bool> createFigure1(
        double length, double width, double height,
        double notchDepth, double notchWidth, double notchHeight,
        double xShift, double yShift,
        QLineEdit* lineEditPosX, QLineEdit* lineEditPosY,
        QLineEdit* lineEditNotchWidth, QLineEdit* lineEditNotchHeight,
        QLineEdit* lineEditLength, QLineEdit* lineEditHeight, double POV_P);

    //
    std::pair<vtkSmartPointer<vtkRenderer>, bool> createFigure2(
        double width, double height, double length,
        double notchWidth, double notchHeight, double notchDepth,
        int rows, int cols, double horizontalSpacing, double verticalSpacing,
        QLineEdit* lineEdit_aperture_height, QLineEdit* lineEdit_pos_vertically,
        QLineEdit* lineEdit_aperture_width, QLineEdit* lineEdit_pos_horizontally, QLineEdit* lineEdit_col_horizontally,
        QLineEdit* lineEdit_size_a, QLineEdit* lineEdit_col_vertically, QLineEdit* lineEdit_size_b,
        QLineEdit* lineEditNotchWidth, QLineEdit* lineEditNotchHeight,
        QLineEdit* lineEditLength, QLineEdit* lineEditHeight, double POV_P);

    // Метод для создания цилиндра с апертурой
    vtkSmartPointer<vtkRenderer> createFigure3(double height, double radius, double notchHeight, double notchRadius, double POV_P);

private:
    vtkSmartPointer<vtkNamedColors> colors; // Для задания цветов

    // Метод для подсветки ошибок в QLineEdit
    void highlightError(QLineEdit* lineEdit, bool isError);

    vtkSmartPointer<vtkCubeSource> createRectangle(double width, double height, double length);
    vtkSmartPointer<vtkCubeSource> createNotch(double notchWidth, double notchHeight, double notchDepth);
};

#endif // MODELSFIGURE_H
