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

vtkSmartPointer<vtkRenderer> ModelsFigure::createFigure1(
    double length, double width, double height,
    double notchDepth, double notchWidth, double notchHeight,
    double xShift, double yShift,
    QLineEdit* lineEditPosX, QLineEdit* lineEditPosY,
    QLineEdit* lineEditNotchWidth, QLineEdit* lineEditNotchHeight,
    QLineEdit* lineEditLength, QLineEdit* lineEditHeight)
{


    // Проверки и подсветка ошибок
    if (xShift + notchDepth >= length && xShift != 0)
    {
        xShift = 0.0001;
        notchDepth = length / 4;

        highlightError(lineEditPosX, true);
        highlightError(lineEditNotchWidth, true);
        highlightError(lineEditLength, true);
    }
    else
    {
        highlightError(lineEditPosX, false);
        highlightError(lineEditNotchWidth, false);
        highlightError(lineEditLength, false);
    }

    if (yShift + notchHeight >= height && yShift != 0)
    {
        yShift = 0.0001;
        notchHeight = height / 4;

        highlightError(lineEditPosY, true);
        highlightError(lineEditNotchHeight, true);
        highlightError(lineEditHeight, true);
    }
    else
    {
        highlightError(lineEditPosY, false);
        highlightError(lineEditNotchHeight, false);
        highlightError(lineEditHeight, false);
    }

    if (notchHeight >= height)
    {
        notchHeight = height / 4;
        highlightError(lineEditNotchHeight, true);
        highlightError(lineEditHeight, true);
    }
    else
    {
        highlightError(lineEditNotchHeight, false);
        highlightError(lineEditHeight, false);
    }

    if (notchDepth >= length)
    {
        notchDepth = length / 4;
        highlightError(lineEditNotchWidth, true);
        highlightError(lineEditLength, true);
    }
    else
    {
        highlightError(lineEditNotchWidth, false);
        highlightError(lineEditLength, false);
    }

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
        -(length / 2) + (notchDepth / 2) + xShift, // Смещение по X
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

    // Настройка рендера
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("White").GetData());
    renderer->AddActor(rectangleActor);
    renderer->AddActor(notchActor);

    return renderer;
}

void ModelsFigure::createFigure2(vtkRenderer* renderer,
                                       double width, double height, double length,
                                       double notchWidth, double notchHeight, double notchDepth,
                                       int rows, int cols, double horizontalSpacing, double verticalSpacing,
                                 QLineEdit* lineEdit_aperture_height, QLineEdit* lineEdit_pos_vertically,
                                 QLineEdit* lineEdit_aperture_width, QLineEdit* lineEdit_pos_horizontally, QLineEdit* lineEdit_col_horizontally, QLineEdit* lineEdit_size_a,
                                 QLineEdit* lineEdit_col_vertically, QLineEdit* lineEdit_size_b)
{

    // Проверки на размеры сетки
    double totalGridWidth = (cols - 1) * horizontalSpacing + notchWidth;
    double totalGridHeight = (rows - 1) * verticalSpacing + notchHeight;

    bool intersectionDetected = false;

    /// Проверка пересечений и ошибок
    if (notchHeight >= verticalSpacing) {
        highlightError(lineEdit_aperture_height, true);
        highlightError(lineEdit_pos_vertically, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_aperture_height, /*false*/true);
        highlightError(lineEdit_pos_vertically, false);
    }

    if (notchWidth >= horizontalSpacing) {
        highlightError(lineEdit_aperture_width, true);
        highlightError(lineEdit_pos_horizontally, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_aperture_width, /*false*/true);
        highlightError(lineEdit_pos_horizontally, false);
    }

    if (totalGridWidth > length) {
        highlightError(lineEdit_col_horizontally, true);
        highlightError(lineEdit_pos_horizontally, true);
        highlightError(lineEdit_size_a, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_col_horizontally, false);
        highlightError(lineEdit_pos_horizontally, false);
        highlightError(lineEdit_size_a, false);
    }

    if (totalGridHeight > height) {
        highlightError(lineEdit_col_vertically, true);
        highlightError(lineEdit_pos_vertically, true);
        highlightError(lineEdit_size_b, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_col_vertically, false);
        highlightError(lineEdit_pos_vertically, false);
        highlightError(lineEdit_size_b, false);
    }

    if (notchWidth >= length) {
        highlightError(lineEdit_aperture_width, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_aperture_width, false);
    }

    if (notchHeight >= height) {
        highlightError(lineEdit_aperture_height, true);
        intersectionDetected = true;
    } else {
        highlightError(lineEdit_aperture_height, false);
    }

    // Если ошибки найдены, прерываем выполнение
    if (intersectionDetected) {
        return;
    }

    // Создание прямоугольника
    vtkSmartPointer<vtkCubeSource> rectangleSource = createRectangle(width, height, length);
    vtkNew<vtkPolyDataMapper> rectangleMapper;
    rectangleMapper->SetInputConnection(rectangleSource->GetOutputPort());

    vtkNew<vtkActor> rectangleActor;
    rectangleActor->SetMapper(rectangleMapper);
    rectangleActor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
    renderer->AddActor(rectangleActor);



    // Создание вырезов (notch)
    vtkSmartPointer<vtkCubeSource> notchSource = createNotch(notchWidth, notchHeight, notchDepth);

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
}


// Метод для создания цилиндра с апертурой
vtkSmartPointer<vtkRenderer> ModelsFigure::createFigure3(double height, double radius, double notchHeight, double notchRadius)
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

    // Создание рендера
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);
    renderer->AddActor(notchActor);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    return renderer;
}
