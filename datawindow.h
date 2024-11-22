#ifndef DATAWINDOW_H
#define DATAWINDOW_H

class DataWindow
{
public:
    DataWindow();

    // Геттеры
    double get_win_f_min() const;
    double get_win_f_max() const;
    double get_win_Points() const;

    double get_win_n() const;
    double get_win_m() const;

    double get_win_P() const;
    double get_win_P_step() const;
    double get_win_integral() const;
    bool get_win_Runge() const;

    double get_win_t() const;

    double get_win_a() const;
    double get_win_b() const;
    double get_win_d() const;

    double get_win_l() const;
    double get_win_w() const;

    double get_win_X() const;
    double get_win_Y() const;

    double get_win_dV() const;
    double get_win_dH() const;
    double get_win_map() const;
    double get_win_nap() const;

    double get_win_r() const;
    double get_win_h() const;

    double get_win_R() const;


    // Сеттеры
    void set_win_f_min(double value);
    void set_win_f_max(double value);
    void set_win_Points(double value);

    void set_win_n(double value);
    void set_win_m(double value);

    void set_win_P(double value);
    void set_win_P_step(double value);
    void set_win_integral(double value);
    void set_win_Runge(bool value);

    void set_win_t(double value);

    void set_win_a(double value);
    void set_win_b(double value);
    void set_win_d(double value);

    void set_win_l(double value);
    void set_win_w(double value);

    void set_win_X(double value);
    void set_win_Y(double value);

    void set_win_dV(double value);
    void set_win_dH(double value);
    void set_win_map(double value);
    void set_win_nap(double value);

    void set_win_r(double value);
    void set_win_h(double value);

    void set_win_R(double value);


private:
    // Источник

        // Общие
    double win_f_min; // lineEdit_Source_Fmin
    double win_f_max; // lineEdit_Source_Fmax
    double win_Points; // lineEdit_Source_NofPoints

        // 1 and 2
    double win_n; // lineEdit_Source_n
    double win_m; // lineEdit_Source_m

    // Точка наблюдения
    double win_P; // lineEdit_POV_P
    double win_P_step; // lineEdit_POV_NofPoints
    double win_integral; // lineEdit_POV_step
    bool win_Runge; // checkBox_Runge

    // Моделирование

        // Общие

            // Корпус
    double win_t; // lineEdit

        // 1 and 2

            // Корпус
    double win_a; // lineEdit_size_a
    double win_b; // lineEdit_size_b
    double win_d; // lineEdit_size_d

            // Аппертура
    double win_l; // lineEdit_aperture_height
    double win_w; // lineEdit_aperture_width

        // 1

            // Аппертура
    double win_X; // lineEdit_pos_x
    double win_Y; // lineEdit_pos_y

        // 2

            // Аппертура
    double win_dV; // lineEdit_pos_vertically
    double win_dH; // lineEdit_pos_horizontally
    double win_map; // lineEdit_col_horizontally
    double win_nap; // lineEdit_col_vertically

        // 3

            // Корпус
    double win_r; // label_size_b
    double win_h; // label_size_a

            // Аппертура
    double win_R; // label_aperture_height
};
#endif // DATAWINDOW_H
