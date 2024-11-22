#include "datawindow.h"

// Конструктор с инициализацией параметров по умолчанию
DataWindow::DataWindow()
    : win_f_min(0.0),
    win_f_max(10.0),
    win_Points(100),
    win_n(1.0),
    win_m(1.0),
    win_P(0.0),
    win_P_step(1.0),
    win_integral(0.0),
    win_Runge(false),
    win_t(0.0),
    win_a(1.0),
    win_b(1.0),
    win_d(1.0),
    win_l(1.0),
    win_w(1.0),
    win_X(0.0),
    win_Y(0.0),
    win_dV(1.0),
    win_dH(1.0),
    win_map(10),
    win_nap(10),
    win_r(1.0),
    win_h(1.0),
    win_R(1.0)
{
}

// Геттеры
double DataWindow::get_win_f_min() const { return win_f_min; }
double DataWindow::get_win_f_max() const { return win_f_max; }
double DataWindow::get_win_Points() const { return win_Points; }

double DataWindow::get_win_n() const { return win_n; }
double DataWindow::get_win_m() const { return win_m; }

double DataWindow::get_win_P() const { return win_P; }
double DataWindow::get_win_P_step() const { return win_P_step; }
double DataWindow::get_win_integral() const { return win_integral; }
bool DataWindow::get_win_Runge() const { return win_Runge; }

double DataWindow::get_win_t() const { return win_t; }

double DataWindow::get_win_a() const { return win_a; }
double DataWindow::get_win_b() const { return win_b; }
double DataWindow::get_win_d() const { return win_d; }

double DataWindow::get_win_l() const { return win_l; }
double DataWindow::get_win_w() const { return win_w; }

double DataWindow::get_win_X() const { return win_X; }
double DataWindow::get_win_Y() const { return win_Y; }

double DataWindow::get_win_dV() const { return win_dV; }
double DataWindow::get_win_dH() const { return win_dH; }
double DataWindow::get_win_map() const { return win_map; }
double DataWindow::get_win_nap() const { return win_nap; }

double DataWindow::get_win_r() const { return win_r; }
double DataWindow::get_win_h() const { return win_h; }

double DataWindow::get_win_R() const { return win_R; }


// Сеттеры
void DataWindow::set_win_f_min(double value)  {  win_f_min = value; }
void DataWindow::set_win_f_max(double value)  {  win_f_max = value; }
void DataWindow::set_win_Points(double value)  {  win_Points = value; }

void DataWindow::set_win_n(double value)  {  win_n = value; }
void DataWindow::set_win_m(double value)  {  win_m = value; }

void DataWindow::set_win_P(double value)  {  win_P = value; }
void DataWindow::set_win_P_step(double value)  {  win_P_step = value; }
void DataWindow::set_win_integral(double value)  {  win_integral = value; }
void DataWindow::set_win_Runge(bool value)  {  win_Runge = value; }

void DataWindow::set_win_t(double value)  {  win_t = value; }

void DataWindow::set_win_a(double value)  {  win_a = value; }
void DataWindow::set_win_b(double value)  {  win_b = value; }
void DataWindow::set_win_d(double value)  {  win_d = value; }

void DataWindow::set_win_l(double value)  {  win_l = value; }
void DataWindow::set_win_w(double value)  {  win_w = value; }

void DataWindow::set_win_X(double value)  {  win_X = value; }
void DataWindow::set_win_Y(double value)  {  win_Y = value; }

void DataWindow::set_win_dV(double value)  {  win_dV = value; }
void DataWindow::set_win_dH(double value)  {  win_dH = value; }
void DataWindow::set_win_map(double value)  {  win_map = value; }
void DataWindow::set_win_nap(double value)  {  win_nap = value; }

void DataWindow::set_win_r(double value)  {  win_r = value; }
void DataWindow::set_win_h(double value)  {  win_h = value; }

void DataWindow::set_win_R(double value)  {  win_R = value; }
