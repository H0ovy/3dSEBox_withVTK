#include "modelsfigure.h"

ModelsFigure::ModelsFigure()
    : colors(vtkSmartPointer<vtkNamedColors>::New())
{
}

vtkSmartPointer<vtkCubeSource> ModelsFigure::createRectangle(double width, double height, double length) {
    vtkSmartPointer<vtkCubeSource> rectangleSource = vtkSmartPointer<vtkCubeSource>::New();
    rectangleSource->SetYLength(height);
    rectangleSource->SetXLength(length);
    rectangleSource->SetZLength(width);
    rectangleSource->Update();
    return rectangleSource;
}

vtkSmartPointer<vtkCubeSource> ModelsFigure::createNotch(double notchWidth, double notchHeight, double notchDepth) {
    vtkSmartPointer<vtkCubeSource> notchSource = vtkSmartPointer<vtkCubeSource>::New();
    notchSource->SetYLength(notchHeight);
    notchSource->SetXLength(notchWidth);
    notchSource->SetZLength(notchDepth);
    notchSource->Update();
    return notchSource;
}

void ModelsFigure::highlightError(QLineEdit* lineEdit, bool isError)
{
    QPalette palette;
    if (isError)
    {
        palette.setColor(QPalette::Base, Qt::red);
    }
    else
    {
        palette.setColor(QPalette::Base, Qt::white);
    }
    lineEdit->setPalette(palette);
}

std::pair<vtkSmartPointer<vtkRenderer>, bool> ModelsFigure::createFigure1(
    double length, double width, double height,
    double notchDepth, double notchWidth, double notchHeight,
    double xShift, double yShift,
    QLineEdit* lineEditPosX, QLineEdit* lineEditPosY,
    QLineEdit* lineEditNotchWidth, QLineEdit* lineEditNotchHeight,
    QLineEdit* lineEditLength, QLineEdit* lineEditHeight, double POV_P)
{

    bool error_happened = false;

    // Проверки и подсветка ошибок
    if (xShift + notchDepth >= length && xShift != 0)
    {
        highlightError(lineEditPosX, true);
        highlightError(lineEditNotchWidth, true);
        highlightError(lineEditLength, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEditPosX, false);
        highlightError(lineEditNotchWidth, false);
        highlightError(lineEditLength, false);
    }

    if (yShift + notchHeight >= height && yShift != 0)
    {
        highlightError(lineEditPosY, true);
        highlightError(lineEditNotchHeight, true);
        highlightError(lineEditHeight, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEditPosY, false);
        highlightError(lineEditNotchHeight, false);
        highlightError(lineEditHeight, false);
    }

    if (notchHeight >= height)
    {
        highlightError(lineEditNotchHeight, true);
        highlightError(lineEditHeight, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEditNotchHeight, false);
        highlightError(lineEditHeight, false);
    }

    if (notchDepth >= length)
    {
        highlightError(lineEditNotchWidth, true);
        highlightError(lineEditLength, true);

        error_happened = true;
    }
    else
    {
        highlightError(lineEditNotchWidth, false);
        highlightError(lineEditLength, false);
    }

    after_error:

    // Основной прямоугольный параллелепипед
    vtkSmartPointer<vtkCubeSource> rectangleSource = vtkSmartPointer<vtkCubeSource>::New();
    rectangleSource->SetXLength(length); // Длина
    rectangleSource->SetYLength(height); // Высота
    rectangleSource->SetZLength(width);  // Ширина
    rectangleSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> rectangleMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    rectangleMapper->SetInputConnection(rectangleSource->GetOutputPort());

    vtkSmartPointer<vtkActor> rectangleActor = vtkSmartPointer<vtkActor>::New();
    rectangleActor->SetMapper(rectangleMapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    // Вырез (апертура)
    vtkSmartPointer<vtkCubeSource> notchSource = vtkSmartPointer<vtkCubeSource>::New();
    notchSource->SetXLength(notchDepth);  // Глубина выреза
    notchSource->SetYLength(notchHeight); // Высота выреза
    notchSource->SetZLength(notchWidth);  // Ширина выреза
    notchSource->Update();

    vtkSmartPointer<vtkTransform> notchTransform = vtkSmartPointer<vtkTransform>::New();
    notchTransform->Translate(
        -(length / 2) + (notchDepth / 2) + xShift,  // Смещение по X
        -(height / 2) + (notchHeight / 2) + yShift, // Смещение по Y
        (width / 2) - 0.00099                       // Смещение по Z
        );

    vtkSmartPointer<vtkTransformPolyDataFilter> notchTransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    notchTransformFilter->SetInputConnection(notchSource->GetOutputPort());
    notchTransformFilter->SetTransform(notchTransform);
    notchTransformFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> notchMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    notchMapper->SetInputConnection(notchTransformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> notchActor = vtkSmartPointer<vtkActor>::New();
    notchActor->SetMapper(notchMapper);
    notchActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkPolyDataMapper> POVmapper;

    vtkNew<vtkSphereSource> POVsource;
    POVmapper->SetInputConnection(POVsource->GetOutputPort());
    POVsource->SetCenter(-(length / 2), -(height / 2), (width / 2) - POV_P);
    POVsource->SetRadius(0.005);

    vtkNew<vtkActor> POVactor;
    POVactor->SetMapper(POVmapper);
    POVactor->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());

    // Настройка рендера
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("White").GetData());
    renderer->AddActor(rectangleActor);
    renderer->AddActor(notchActor);
    renderer->AddActor(POVactor);

    return std::pair<vtkSmartPointer<vtkRenderer>, bool>(renderer, error_happened);
}

std::pair<vtkSmartPointer<vtkRenderer>, bool> ModelsFigure::createFigure2(
                                       double width, double height, double length,
                                       double notchWidth, double notchHeight, double notchDepth,
                                       int rows, int cols, double horizontalSpacing, double verticalSpacing,
                                 QLineEdit* lineEdit_aperture_height, QLineEdit* lineEdit_pos_vertically,
                                 QLineEdit* lineEdit_aperture_width, QLineEdit* lineEdit_pos_horizontally, QLineEdit* lineEdit_col_horizontally, QLineEdit* lineEdit_size_a,
                                 QLineEdit* lineEdit_col_vertically, QLineEdit* lineEdit_size_b,
    QLineEdit* lineEditNotchWidth, QLineEdit* lineEditNotchHeight,    QLineEdit* lineEditLength, QLineEdit* lineEditHeight, double POV_P)
{

    // Проверки на размеры сетки
    double totalGridWidth = (cols - 1) * horizontalSpacing + notchWidth;
    double totalGridHeight = (rows - 1) * verticalSpacing + notchHeight;

    bool error_happened = false;

    /// Проверка пересечений и ошибок
    if (notchHeight >= height)
    {
        highlightError(lineEditNotchHeight, true);
        highlightError(lineEditHeight, true);

        error_happened = true;
        goto after_basic_check;
    }
    else
    {
        highlightError(lineEditNotchHeight, false);
        highlightError(lineEditHeight, false);
    }

    if (notchWidth >= length)
    {
        highlightError(lineEditNotchWidth, true);
        highlightError(lineEditLength, true);

        error_happened = true;
    }
    else
    {
        highlightError(lineEditNotchWidth, false);
        highlightError(lineEditLength, false);
    }

    after_basic_check:

    if (notchHeight >= verticalSpacing)
    {
        highlightError(lineEdit_aperture_height, true);
        highlightError(lineEdit_pos_vertically, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEdit_aperture_height, false);
        highlightError(lineEdit_pos_vertically, false);
    }

    if (notchWidth >= horizontalSpacing)
    {
        highlightError(lineEdit_aperture_width, true);
        highlightError(lineEdit_pos_horizontally, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEdit_aperture_width, false);
        highlightError(lineEdit_pos_horizontally, false);
    }

    if (totalGridWidth > length)
    {
        highlightError(lineEdit_col_horizontally, true);
        highlightError(lineEdit_pos_horizontally, true);
        highlightError(lineEdit_size_a, true);

        error_happened = true;
        goto after_error;
    }
    else
    {
        highlightError(lineEdit_col_horizontally, false);
        highlightError(lineEdit_pos_horizontally, false);
        highlightError(lineEdit_size_a, false);
    }

    if (totalGridHeight > height)
    {
        highlightError(lineEdit_col_vertically, true);
        highlightError(lineEdit_pos_vertically, true);
        highlightError(lineEdit_size_b, true);

        error_happened = true;
    }
    else
    {
        highlightError(lineEdit_col_vertically, false);
        highlightError(lineEdit_pos_vertically, false);
        highlightError(lineEdit_size_b, false);
    }

    after_error:

    // Создание прямоугольника
    vtkSmartPointer<vtkCubeSource> rectangleSource = createRectangle(width, height, length);
    vtkNew<vtkPolyDataMapper> rectangleMapper;
    rectangleMapper->SetInputConnection(rectangleSource->GetOutputPort());

    vtkNew<vtkActor> rectangleActor;
    rectangleActor->SetMapper(rectangleMapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    vtkNew<vtkPolyDataMapper> POVmapper;

    vtkNew<vtkSphereSource> POVsource;
    POVmapper->SetInputConnection(POVsource->GetOutputPort());
    POVsource->SetCenter(-(length / 2), -(height / 2), (width / 2) - POV_P);
    POVsource->SetRadius(0.005);

    vtkNew<vtkActor> POVactor;
    POVactor->SetMapper(POVmapper);
    POVactor->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(rectangleActor);
    renderer->AddActor(POVactor);

    // Создание вырезов (notch)
    vtkSmartPointer<vtkCubeSource> notchSource = createNotch(notchWidth, notchHeight, notchDepth);

    double gridOffsetX = -(totalGridWidth / 2.0) + (notchWidth / 2.0);
    double gridOffsetY = -(totalGridHeight / 2.0) + (notchHeight / 2.0);


    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
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

    return std::pair<vtkSmartPointer<vtkRenderer>, bool>(renderer, error_happened);
}


// Метод для создания цилиндра с апертурой
vtkSmartPointer<vtkRenderer> ModelsFigure::createFigure3(double height, double radius, double notchHeight, double notchRadius, double POV_P)
{
    // Проверка и корректировка параметров
    if (notchRadius >= radius)
        notchRadius = radius / 4;

    // Создание цилиндра
    vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetHeight(height);
    cylinderSource->SetRadius(radius);
    cylinderSource->SetResolution(40);

    vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinderSource->GetOutputPort());

    vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());

    // Создание апертуры
    vtkSmartPointer<vtkCylinderSource> notchSource = vtkSmartPointer<vtkCylinderSource>::New();
    notchSource->SetHeight(notchHeight);
    notchSource->SetRadius(notchRadius);
    notchSource->SetResolution(40);

    vtkSmartPointer<vtkTransform> notchTransform = vtkSmartPointer<vtkTransform>::New();
    notchTransform->Translate(0, -(height / 2 - notchHeight / 2 + 0.001), 0);

    vtkSmartPointer<vtkTransformPolyDataFilter> notchTransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    notchTransformFilter->SetInputConnection(notchSource->GetOutputPort());
    notchTransformFilter->SetTransform(notchTransform);

    vtkSmartPointer<vtkPolyDataMapper> notchMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    notchMapper->SetInputConnection(notchTransformFilter->GetOutputPort());

    vtkSmartPointer<vtkActor> notchActor = vtkSmartPointer<vtkActor>::New();
    notchActor->SetMapper(notchMapper);
    notchActor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkPolyDataMapper> POVmapper;

    vtkNew<vtkSphereSource> POVsource;
    POVmapper->SetInputConnection(POVsource->GetOutputPort());
    POVsource->SetCenter(0, POV_P-(height / 2)/*-(height / 2)*/, /*(width / 2)*/ -(radius));
    POVsource->SetRadius(0.005);

    vtkNew<vtkActor> POVactor;
    POVactor->SetMapper(POVmapper);
    POVactor->GetProperty()->SetColor(colors->GetColor3d("Blue").GetData());

    // Создание рендера
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);
    renderer->AddActor(notchActor);
    renderer->AddActor(POVactor);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    return renderer;
}
