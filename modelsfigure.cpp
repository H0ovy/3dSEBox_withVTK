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
    rectangleActor->GetProperty()->SetColor(0.498,0.337,0.851);

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
    POVactor->GetProperty()->SetColor(0.18,0.565,0.98);

    vtkNew<vtkVectorText> text_a;
    text_a->SetText("a");
    vtkNew<vtkActor> TextActor_A;
    vtkNew<vtkPolyDataMapper> TextMapper_A;
    TextMapper_A->SetInputConnection(text_a->GetOutputPort());
    TextActor_A->SetMapper(TextMapper_A);
    TextActor_A->SetScale(0.05, 0.05, 0.05);
    //TextActor_A->RotateX(270);
    TextActor_A->SetPosition(-0.025, -(height/2) - 0.06, width/2);
    TextActor_A->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkVectorText> text_b;
    text_b->SetText("b");
    vtkNew<vtkActor> TextActor_B;
    vtkNew<vtkPolyDataMapper> TextMapper_B;
    TextMapper_B->SetInputConnection(text_b->GetOutputPort());
    TextActor_B->SetMapper(TextMapper_B);
    TextActor_B->SetScale(0.05, 0.05, 0.05);
    //TextActor_B->RotateX(270);
    TextActor_B->SetPosition(-(length/2) - 0.06 , -0.025, width/2);
    TextActor_B->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkVectorText> text_d;
    text_d->SetText("d");
    vtkNew<vtkActor> TextActor_D;
    vtkNew<vtkPolyDataMapper> TextMapper_D;
    TextMapper_D->SetInputConnection(text_d->GetOutputPort());
    TextActor_D->SetMapper(TextMapper_D);
    TextActor_D->SetScale(0.05, 0.05, 0.05);
    TextActor_D->RotateY(270);
    TextActor_D->SetPosition(-(length/2), -(height/2) - 0.06, -0.025);
    TextActor_D->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());


    // Настройка рендера
    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(rectangleActor);
    renderer->AddActor(notchActor);
    renderer->AddActor(POVactor);
    renderer->AddActor(TextActor_A);
    renderer->AddActor(TextActor_B);
    renderer->AddActor(TextActor_D);
    renderer->SetBackground(colors->GetColor3d("White").GetData());



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
    rectangleActor->GetProperty()->SetColor(0.498,0.337,0.851);

    vtkNew<vtkPolyDataMapper> POVmapper;

    vtkNew<vtkSphereSource> POVsource;
    POVmapper->SetInputConnection(POVsource->GetOutputPort());
    POVsource->SetCenter(-(length / 2), -(height / 2), (width / 2) - POV_P);
    POVsource->SetRadius(0.005);

    vtkNew<vtkActor> POVactor;
    POVactor->SetMapper(POVmapper);
    POVactor->GetProperty()->SetColor(0.18,0.565,0.98);

    vtkNew<vtkVectorText> text_a;
    text_a->SetText("a");
    vtkNew<vtkActor> TextActor_A;
    vtkNew<vtkPolyDataMapper> TextMapper_A;
    TextMapper_A->SetInputConnection(text_a->GetOutputPort());
    TextActor_A->SetMapper(TextMapper_A);
    TextActor_A->SetScale(0.05, 0.05, 0.05);
    //TextActor_A->RotateX(270);
    TextActor_A->SetPosition(-0.025, -(height/2) - 0.06, width/2);
    TextActor_A->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkVectorText> text_b;
    text_b->SetText("b");
    vtkNew<vtkActor> TextActor_B;
    vtkNew<vtkPolyDataMapper> TextMapper_B;
    TextMapper_B->SetInputConnection(text_b->GetOutputPort());
    TextActor_B->SetMapper(TextMapper_B);
    TextActor_B->SetScale(0.05, 0.05, 0.05);
    //TextActor_B->RotateX(270);
    TextActor_B->SetPosition(-(length/2) - 0.06 , -0.025, width/2);
    TextActor_B->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkVectorText> text_d;
    text_d->SetText("d");
    vtkNew<vtkActor> TextActor_D;
    vtkNew<vtkPolyDataMapper> TextMapper_D;
    TextMapper_D->SetInputConnection(text_d->GetOutputPort());
    TextActor_D->SetMapper(TextMapper_D);
    TextActor_D->SetScale(0.05, 0.05, 0.05);
    TextActor_D->RotateY(270);
    TextActor_D->SetPosition(-(length/2), -(height/2) - 0.06, -0.025);
    TextActor_D->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkRenderer> renderer;
    renderer->AddActor(rectangleActor);
    renderer->AddActor(POVactor);
    renderer->AddActor(TextActor_A);
    renderer->AddActor(TextActor_B);
    renderer->AddActor(TextActor_D);
    renderer->SetBackground(colors->GetColor3d("White").GetData());
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
    cylinderActor->GetProperty()->SetColor(0.498,0.337,0.851);

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
    POVactor->GetProperty()->SetColor(0.18,0.565,0.98);

    vtkNew<vtkVectorText> text_h;
    text_h->SetText("h");
    vtkNew<vtkActor> TextActor_H;
    vtkNew<vtkPolyDataMapper> TextMapper_H;
    TextMapper_H->SetInputConnection(text_h->GetOutputPort());
    TextActor_H->SetMapper(TextMapper_H);
    TextActor_H->SetScale(0.05, 0.05, 0.05);
    //TextActor_A->RotateX(270);
    TextActor_H->SetPosition(-0.025, -(height) - 0.05, radius);
    TextActor_H->SetPosition(-(radius), -0.025, radius);
    TextActor_H->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    vtkNew<vtkVectorText> text_r;
    text_r->SetText("r");
    vtkNew<vtkActor> TextActor_R;
    vtkNew<vtkPolyDataMapper> TextMapper_R;
    TextMapper_R->SetInputConnection(text_r->GetOutputPort());
    TextActor_R->SetMapper(TextMapper_R);
    TextActor_R->SetScale(0.05, 0.05, 0.05);
    TextActor_R->RotateX(90);
    TextActor_R->SetPosition(-0.025, -(height/2), -(radius) - 0.05);
    TextActor_R->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

    // Создание рендера
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);
    renderer->AddActor(notchActor);
    renderer->AddActor(POVactor);
    renderer->AddActor(TextActor_H);
    renderer->AddActor(TextActor_R);
    renderer->SetBackground(colors->GetColor3d("White").GetData());

    return renderer;
}
