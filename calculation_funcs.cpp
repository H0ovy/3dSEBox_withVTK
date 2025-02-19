#include "calculation_funcs.h"


#define _USE_MATH_DEFINES

using namespace std;
typedef complex<double> dcomp;

double Calculation_Funcs::WAMGetal(int* iter, double freq, double R, double r, double t, double d, double p){
    dcomp j(0.0,1.0);
    auto z0=120.0*M_PI;
    dcomp v0(1.0,0.0);

    //эффективная ширина
    auto sqpi=sqrt(M_PI);
    auto we=r*sqpi-(5.0*t/(4.0*M_PI))*(1.0+log(4.0*M_PI*r*sqpi/t));

    //характеристический импеданс
    dcomp kc=pow((we/(2.0*R)),2);
    dcomp z0stemp1=pow((dcomp(1.0,1.0)-kc),(0.25));
    dcomp z0stemp2=log(2.0*(1.0+z0stemp1)/(1.0-z0stemp1));
    auto z0s=120.0*M_PI*M_PI/z0stemp2;
    auto c = 299792458.0;
    ++(*iter);
    auto f=freq;
    auto lambda=c/f;
    // auto k0=2.0*M_PI/lambda;

    // Сопротивление стенки с апертурой
    dcomp zap=j*z0s*(r*sqpi/(4*R))*tan(f*r*sqpi*3.14/c);

    // auto lamc=2.0*M_PI*R/1.841; // для основного типа волн

    // Преобразование в точку A
    dcomp v1=v0*zap/(z0+zap);
    dcomp z1=z0*zap/(z0+zap);
    // dcomp temp = 1.0-pow((lambda/lamc),2);

    // Характеристический импеданс и постоянная распространения в корпусе
    // auto zg=z0/sqrt(temp);
    // auto kg=k0*sqrt(temp);

    // Преобразование в точку P
    // auto v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
    // auto z2=(z1+j*zg*tan(kg*p))/(1.0+j*(z1/zg)*tan(kg*p));

    // // Нагрузка
    // auto z3=j*zg*tan(kg*(d-p));

    // // Напряжение в точке P
    // auto vp=v2*z3/(z2+z3);
    QVector<double> bfr = {1.841,
                           2.405,
                           3.054,
                           3.832,
                           3.832,
                           4.201,
                           5.135,
                           5.331,
                           5.520,
                           6.380,
                           6.706,
                           7.016,
                           7.016,
                           8.015,
                           8.417,
                           8.536,
                           8.650,
                           9.760,
                           9.969,
                           10.173};
    dcomp vpp(0,0);
    for (int n=0;n<20;n++){
        double lamc=2*M_PI*r/bfr[n];

        dcomp zg=z0/(sqrt(dcomp(1,0)-pow((lambda/lamc),2)));
        dcomp kg=(2*M_PI/lambda)*sqrt(dcomp(1,0)-pow((lambda/lamc),2));

        dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
        dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

        dcomp z3=j*zg*tan(kg*(d-p));

        dcomp vp=v2*z3/(z2+z3);
        vpp=vpp+vp;
    }

    return -20*log10(abs(v0/dcomp(2.0,0.0)*vpp));
}


double Calculation_Funcs::calcNIEetal(int* iter,double freq, double w, double l, double xbol, double ybol, double p, double d, double b, double a, double t, double m1, double n1){
    auto mu0=4.0*3.14*pow(10,(-7));
    auto eps0=8.85*pow(10,(-12));
    auto m=1.0;
    dcomp j(0.0,1.0);
    auto n=0.0;
    auto wctemp1=pow((3.14*m/a),2);
    auto wctemp2=pow((3.14*n/b),2);
    auto wc=sqrt((wctemp1+wctemp2)/(eps0*mu0));
    ++(*iter);
    auto z0 = 120.0*M_PI;
    auto c = 299792458.0;
    auto f= freq;
    dcomp v0(1.0,0.0);
    auto lambda=c/freq;
    auto k0=2.0*M_PI/lambda;

    auto gamtemp=pow((2.0*3.14*f),2);
    auto gam=sqrt(mu0*eps0*(wc+gamtemp));

    // проводимость емкостной диафрагмы
    auto yctemp2=3.14*ybol/b;
    auto yctemp3=3.14*w/(2.0*b);
    auto yctemp4=1.0/sin(yctemp2);
    auto yctemp5=1.0/sin(yctemp3);
    yctemp5=log(yctemp4*yctemp5);
    auto yctemp1=(b*pow(gam,2))/(mu0*3.14*3.14*f);
    auto yc=j*yctemp1*yctemp5;

    // Проводимость индуктивной диафрагмы

    auto yltemp1=3.14*xbol/a;
    auto yltemp2=3.14*l/(2.0*a);
    auto yltemp3=1.0/(sin(yltemp1));
    auto yltemp4=1.0/(sin(yltemp2));
        auto yl=(-j/(a*mu0*f))*((pow(yltemp3,2))*(pow(yltemp4,2))-1.0);

    // Сопротивление стенки с апертурой
    auto zap=1.0/(yc+yl+yl);

    // Преобразование в точку A
    auto v1=v0*zap/(z0+zap);
    dcomp z1=z0*zap/(z0+zap);

    // Преобразование в точку A
    dcomp vp1(0,0);

    for (int mm=1; mm<=m1; mm++){
        for (int nn=0; nn<=n1; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vp1=vp+vp1;

        }
    }
    return -20*log10(abs(dcomp(2.0,0.0)*vp1/v0));
}
double Calculation_Funcs::calcSomeRob(int *iter, double freq, double t, double w, double b, double L, double a, double d, double p, double m, double n)
{
    auto c0 = 299792458;
    auto lambda = c0/freq;
    dcomp v0(1.0, 0.0);
    auto z0 = 120 * M_PI;
    dcomp Z0(z0, 0.0);
    ++(*iter);
    auto we = w - ((5 * t * (1 + log(4 * M_PI * w / t))) / (4 * M_PI));
    auto kc = we / b;
    auto temp = pow((1 - kc * kc), 0.25);
    auto temp2 = (2 + 2 * temp) / (1 - temp);
    auto temp3 = M_PI * 1.0 / log(temp2);
    auto Z0s = 120.0 * M_PI * temp3;
    auto k0 = 2.0 * M_PI / lambda;
    auto temp8 = L * Z0s;
    dcomp temp9(0.0, temp8);
    dcomp temp10 = temp9*tan(k0 * L / 2);
    dcomp Zap = temp10 / (2 * a);
    dcomp Ztmp = Z0 + Zap;
    dcomp v1 = v0 * Zap/Ztmp;
    dcomp Z1 = Z0 * Zap / Ztmp;
    // auto temp1 = pow((lambda/(2 * a)), 2);
    // auto tmp_zg = dcomp(1.0,0.0) - dcomp(temp1,0.0);
    // dcomp Zg = dcomp(z0,0) / ( sqrt(tmp_zg));
    // auto tmp_kg = dcomp(1.0,0.0) - dcomp(temp1,0.0);
    // dcomp kg = dcomp(k0, 0.0) * sqrt(tmp_kg);
    // dcomp temp22 = kg * p;
    // dcomp temp32 = cos(temp22) + dcomp(0.0, 1.0) * Z1 / Zg * sin(temp22);
    // // dcomp v2 = v1 / temp32;
    // dcomp temp4 = tan(temp22);
    // dcomp Z_temp = dcomp(0.0, 1.0) *  Zg * temp4;
    // dcomp Z_temp2 = dcomp(0.0, 1.0) * Z1 * temp4/Zg;
    // dcomp Z2 = (Z1 + Z_temp) / (dcomp(1.0, 0.0) + Z_temp2);
    // dcomp Z3 = dcomp(0.0, 1.0) * Zg * tan(kg * (d - p));
    // dcomp vp = (v2 * Z3) / (Z2 + Z3);


    // Преобразование в точку A
    dcomp vp1(0,0);
    dcomp j(0.0,1.0);
    for (int mm=1; mm<=m; mm++){
        for (int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(Z1/zg)*sin(kg*p));
            dcomp z2=(Z1 + j * zg * tan(kg * p)) / (dcomp(1, 0) + j * (Z1 / zg) * tan(kg * p));

            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp= v2 * z3 / (z2+z3);
            vp1=vp+vp1;
        }
    }
    return -20 * log10(abs(dcomp(2.0, 0.0) * vp1 / v0));
}
double Calculation_Funcs::func(double x, double y, double w, double L, double m, double n, double a, double b)
{
    double temp4 = (a-L)/2;
    double temp6=(b-w)/2;
    return sin((M_PI*m*x)/a) * sin((M_PI*m*(x-temp4))/L)* cos((M_PI*n*y)/b)* cos((M_PI*n*(y-temp6))/w);
}
double Calculation_Funcs::func2(double x, double m, double a, double L)
{
    auto temp4=(a-L)/2;
    return (sin(M_PI*m*x/a) * sin((M_PI*m*(x-temp4))/L));
}
double Calculation_Funcs::func3(double y, double n, double b, double w)
{
    auto temp6=(b-w)/2;
    return (cos(M_PI*n*y/b) * cos((M_PI*n*(y-temp6))/w));
}
double Calculation_Funcs::doubleintegral(int *iter,double a, double b, double c, double d, double nx, double ny, double w, double L, double m,double n)
{
    //variable to mesure the time of execution
    QElapsedTimer timer;
    double hx = (b - a)/(nx);
    double hy = (d - c)/(ny);
    double xi, yj;
    double I = 0;

    timer.start();

    //#pragma omp parallel for schedule(dynamic, 100)
        for(int i=0; i<(int)nx; i++)
        {
            for(int j=0; j<ny; j++){
                xi = a + hx/2 + i*hx;
                yj = c + hy/2 + j*hy;
                ++(*iter);
                I += hx*hy*func(xi,yj,w,L,m,n,a,b);
            }
        }
    return I;
}

// 2018 Ivanov
double Calculation_Funcs::ren(int *iter,double freq, double a, double b,double p, double d, double t, double w, double L, double nap, double map, double m,double n){
    const double c=2.99792458*pow(10,8);
    ++(*iter);
    auto lambda=c/freq;
    auto z0=120*M_PI;                // free-space characteristic impedance
    dcomp v0(1.0,0.0);     // напряжение эквивал. источника
    auto q=nap*map;       // общее число апетур в массиве

    //-------- REN_2016 ЗАМЕНА МАССИВА ОДНОЙ АПЕРТУРОЙ -------------------
        double sdf=1.283*pow(q,(-0.1407))-0.2829;   // scale down factor
        auto s=w*L*q;                    // area of the aperture array
        auto nu=w/L;                     // aspect ratio
            auto ww=sqrt(s/nu);
            auto ll=nu*ww;
        double l1=sdf*ll;                       // length (equivalent aperture)
        double w1=sdf*ww;                      // whidth (equivalent aperture)

        // Эффективная ширина
        auto we=w1-((5*t)/(4*M_PI))*(1+log(4*M_PI*w1/t));

        // Характеристический импеданс эквивалентной линии передачи
        auto ke=we/b;
        auto z0s=120.0*M_PI*M_PI*pow((log(2*(1+pow((1-pow(ke,2)),0.25))/(1-pow((1-pow(ke,2)),0.25)))),(-1));

    double k0=2.0*M_PI/lambda;

    // Сопротивление стенки с апертурой
    dcomp zap(0.0,0.5*(l1/a)*z0s*tan(k0*l1/2));
    dcomp j(0.0,1.0);

    // Преобразование в точку A
    dcomp v1=v0*zap/(z0+zap);
    // auto test1=(z0*zap);
    // auto test2= (z0+zap);
    // auto test3=test1/test2;
    dcomp z1=(z0*zap)/(z0+zap);
    dcomp vpp(0,0); // суммарное напряжение в точке наблюдения

    // Эти циклы для учета высших мод TEmn (mm,nn за номер моды):
    dcomp zg = 0;
    dcomp kg = 0;
    dcomp v2 = 0;
    dcomp z2 = 0;
    dcomp z3 = 0;
    std::complex<double> vp = 0;
    //variable to mesure the time of execution
    QElapsedTimer timer;

    timer.start();
        for (int mm=1; mm<=(int)m; mm++) {
            for (int nn=0; nn<=n; nn++) {
                // Характеристический импеданс и постоянная распространения в корпусе
                zg=z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),dcomp(2,0))-pow((lambda*nn/(dcomp(2,0)*b)),dcomp(2,0)));
                kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),dcomp(2,0))-pow((lambda*nn/(dcomp(2,0)*b)),dcomp(2,0)));
                 ++(*iter);

                // Преобразование в точку P
                v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
                z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

                // Нагрузка
                z3=j*zg*tan(kg*(d-p));

                // Напряжение в точке P
                vp=v2*z3/(z2+z3);
                vpp=vp+vpp;
            }
        }
    return (-20*log10(abs(dcomp(2.0,0.0)*vpp/v0)));
}
double Calculation_Funcs::fact(double N)
{
    if(N < 0) // если пользователь ввел отрицательное число
        return 0; // возвращаем ноль
    if (N == 0) // если пользователь ввел ноль,
        return 1; // возвращаем факториал от нуля
    else // Во всех остальных случаях
        return N * fact(N - 1); // делаем рекурсию.
}
double Calculation_Funcs::Dehkhoda_2007(int *iter, double freq, double a, double b,double p, double d, double w, double nap, double map, double m,double n, double dh, double dv)
{
    auto dd=w;
    ++(*iter);
    auto larr=dh+(map-1)*dh;
    auto warr=dv+(nap-1)*dv;
    dcomp j(0.0,1.0);
    dcomp v0(1.0,0.0);
    auto z0=120*M_PI;
    auto y0=1/z0;

    // Аргумент функции Бесселя
    auto xx=pow((M_PI*dd*(pow(m,2)/pow(dh,2)+pow(n,2)/pow(dv,2))*0.5),0.5)/pow((pow(m,2)/pow(dh,2)+pow(n,2)/pow(dv,2)),2.5);
    const double c=2.99792458*pow(10,8);
    auto lambda=c/freq;
    double j1=0, jx;
    auto k0=2.0*M_PI/lambda;
            auto s=1;

            // Функция Бесселя
            for(int gg=0; gg<=s; gg++){
                jx=(pow((-1),gg))*(pow(xx,(2*gg+1)))/(pow(2,(2*gg+1))*fact(gg)*fact(gg+1));
                j1=j1+jx;               
            }

            dcomp temp1=-j*dcomp(3,0)*dh*dv*lambda/(M_PI*pow(dd,3));
            dcomp temp2=dcomp(288,0)*j/(M_PI*lambda*pow(dd,2));
            dcomp temp3(0,0);
            dcomp temp4;

            for(int mm=0; mm<=m; mm=mm+2)
            {
                for(int nn=0; nn<=n; nn=nn+2)
                {
                    ++(*iter);
                    if ((mm==0)&&(nn==0)){
                        dcomp epsn(1,0); dcomp epsm(1,0);
                        temp4=(epsm*pow(nn,2)/pow(dv,2)+epsn*pow(mm,2)/pow(dh,2))*jx;}
                    else if ((mm==0)&&(nn>0)){
                        dcomp epsn(1,0); dcomp epsm(2,0);
                        temp4=(epsm*pow(nn,2)/pow(dv,2)+epsn*pow(mm,2)/pow(dh,2))*jx;}
                    else if ((mm>0)&&(nn==0)){
                        dcomp epsn(2,0); dcomp epsm(1,0);
                        temp4=(epsm*pow(nn,2)/pow(dv,2)+epsn*pow(mm,2)/pow(dh,2))*jx;}
                    else if ((mm>0)&&(nn>0)){
                        dcomp epsn(2,0); dcomp epsm(2,0);
                        temp4=(epsm*pow(nn,2)/pow(dv,2)+epsn*pow(mm,2)/pow(dh,2))*jx;}
                    temp3=temp4+temp3;
                }
            }
            dcomp temp5=temp1+temp2*temp3;
            dcomp yah=temp5*y0;

             // Импеданс перфорированной стенки
            auto zah=dcomp(1,0)/yah;
            auto sarr=warr*larr;  // площадь покрытия массивом
            auto sfw=a*b;         // площадь передней стенки
            auto zap=zah*sarr/sfw;

            // Преобразование в точку A
            dcomp v1=v0*zap/(z0+zap);
            dcomp z1=z0*zap/(z0+zap);

    dcomp vpp(0,0);
    dcomp zg(0,0);
    dcomp kg(0,0);
    dcomp v2(0,0);
    dcomp z2(0,0);
    dcomp z3(0,0);
    dcomp vp(0,0);

    QElapsedTimer timer;

    timer.start();

    for(int mm=1; mm<=m; mm++){
        for(int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            zg=z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

            // Нагрузка
            z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            vp=v2*z3/(z2+z3);
            vpp=vp+vpp;
        }
    }
    auto SEDeh=-20*log10(abs(dcomp(2.0,0.0)*vpp/v0));

    return SEDeh;
}

double Calculation_Funcs::Nie_2017(int *iter, double freq, double a, double b,double p, double d, double t, double w, double nap, double map, double m,double n, double dh, double dv)
{
    auto larr=dh+(map-1)*dh;
    auto warr=dv+(nap-1)*dv;
    auto dd=w;
    dcomp j(0.0,1.0);
    dcomp v0(1.0,0.0);
    auto z0=120*M_PI;
    int polariz=1;  // Это условие для выбора типа поляризации.
                // Если =1, тогда перпендикулярная
                // Если =0, тогда параллельная

    double tetta=0;    // угол падения волны
    const double c=2.99792458*pow(10,8);
    auto lambda=c/freq;
    auto k0=2.0*M_PI/lambda;
    dcomp zah;
    ++(*iter);

    // Сопротивление стенки с апертурой
    if (polariz==1) { // вынести !!!
        auto temp11=(3*dh*dv*lambda)/(16*M_PI*cos(tetta)*pow((dd/2),3));
        zah= pow(10.0,((-0.8*t)/(dd/2.0)))*(j*z0/2.0)*pow((1.0+pow(temp11,2.0)),-0.5);
    }
    else {
        auto temp22=(3*dh*dv*lambda*cos(tetta))/(16*M_PI*pow((dd/2),3));
        zah= pow(10.0,(-0.8*t/(dd/2.0)))*(j*z0/2.0)*pow(pow((1.0+temp22),2.0),-0.5);
    }
            auto sarr=warr*larr;  // площадь покрытия массивом
            auto sfw=a*b;         // площадь передней стенки
            dcomp zap=zah*sarr/sfw;


    // Преобразование в точку A
    dcomp v1=v0*zap/(z0+zap);
    dcomp z1=z0*zap/(z0+zap);
    dcomp vp1(0,0);

    for (int mm=1; mm<=m; mm++){
        for (int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vp1=vp+vp1;
        }
    }
    return -20*log10(abs(dcomp(2.0,0.0)*vp1/v0));
}
double Calculation_Funcs::integral(double min, double max, double n, double m, double a, double L, int n_of_func)
{
    auto h = (max - min)/n;
    double result;
    if(n_of_func == 2)
    {
        result = 0.5 * (func2(min,m,a,L) + func2(max,m,a,L));
        for (int i=1; i<n; i++){
            result += func2(min + i*h,m,a,L);
        }
    }
    else if(n_of_func == 3)
    {
        result = 0.5 * (func3(min,m,a,L) + func3(max,m,a,L));
        for (int i=1; i<n; i++){
            result += func3(min + i*h,m,a,L);
        }
    }
    result *= h;
    return result;
}
double Calculation_Funcs::calcsomeYongshi(int *iter,double freq, double t, double w, double b, double L, double a, double d, double p, double xbol, double ybol, double n, double m, double sigma, double intval, bool RungeVal)
{
    auto c0 = 299792458;
    auto mue_r =  1.000023;
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    dcomp Z0((120*M_PI),0.0);
    dcomp j(0.0,1.0);
    auto kc = (w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI))) / b;
    auto Z0s = 120*M_PI*M_PI* (1/log((2+2*(pow((1-kc*kc),0.25))) / (1-(pow((1-kc*kc),0.25)))));
    auto k0 = 2.0*M_PI / lambda;
    dcomp ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);
    auto xmin=(a-L)/2;
    auto xmax=((a-L)/2)+L;
    auto ymin=(b-w)/2;
    auto ymax=((b-w)/2)+w;
    ++(*iter);
    double temp8 = 0;

    if(RungeVal){
        int integval=2;
        double temp88;
        temp8=doubleintegral(iter,ymin,ymax,xmin,xmax,integval,integval,w,L,m,n);
        do
        {
            temp88=temp8;
            integval=integval+2;
            temp8=doubleintegral(iter,ymin,ymax,xmin,xmax,integval,integval,w,L,m,n);
        }
        while(abs(temp8-temp88)>0.001);
    }
    else
        temp8=doubleintegral(iter,ymin,ymax,xmin,xmax,intval,intval,w,L,m,n);

    auto Cma=temp8/(xbol*ybol);
    dcomp temp9= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    dcomp Zap = Cma / 2 * j * Z0s * temp9;
    // auto temp10 = pow((lambda*m/(2*a)),2);
    // auto temp11 = pow((lambda*n/(2*b)),2);
    // dcomp tmp_zg = dcomp(1.0,0.0)-dcomp(temp11,0.0)-dcomp(temp10,0.0);
    // dcomp Zg = Z0 / ( sqrt(tmp_zg));
    // dcomp kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    // dcomp temp12 = kg * p;
    // dcomp v2 = (v0 * Zap/(Z0 + Zap)) / (cos(temp12) + j*( Z0 * Zap / (Z0 + Zap))/Zg*sin(temp12));
    // dcomp Z21=( Z0 * Zap / (Z0 + Zap))+sqrt(dcomp(-1.0,0.0))*Zg*(tan((kg * p)));
    // dcomp Z22=1.0+sqrt(dcomp(-1.0,0.0))*(( Z0 * Zap / (Z0 + Zap))/Zg)*(tan((kg * p)));
    // dcomp Z3 = Zg*(ZL+j*Zg*tan(kg*(d-p)))/(Zg+j*ZL*tan(kg*(d-p)));
    // dcomp vp = (v2*Z3) / ((Z21/Z22)+Z3);



    // Преобразование в точку A
    dcomp v1=v0*Zap/(Z0+Zap);
    dcomp z1=Z0*Zap/(Z0+Zap);
    dcomp vp1(0,0);

    for (int mm=1; mm<=m; mm++){
        for (int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=Z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));
            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vp1=vp+vp1;

        }
    }

    return -20*log10(abs(dcomp(2.0,0.0)*vp1/v0));

}

double Calculation_Funcs::CalcTemp(int *iter, bool RungeVal, double xmax, double xmin, double temp6, double m, double a, double L, double intval)
{
    if(RungeVal)
    {
        int integval=2;
        double temp66;
        temp6=integral(xmin,xmax,integval,m,a,L,2);
        do{
            temp66=temp6;
            ++(*iter);
            integval=integval+2;
            temp6=integral(xmin,xmax,integval,m,a,L,2);
        } while((abs(temp6-temp66))>0.001);
    }
    else{
        temp6=integral(xmin,xmax,intval,m,a,L,2);
        ++(*iter);
    }
    return temp6;
}

double Calculation_Funcs::calcsomePoad(int *iter,double freq, double t, double w, double b, double L, double a, double d, double p, double xbol, double n, double m, double sigma, double intval, bool RungeVal)
{
    auto c0 = 299792458;
    ++(*iter);
    auto mue_r =  1.000023;
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    auto z0 = 120*M_PI;
    dcomp Z0(z0,0.0);
    dcomp j(0.0,1.0);
    auto we = w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI));
    auto kc = we / b;
    auto temp = pow((1-kc*kc),0.25);
    auto temp2 = (2+2*temp) / (1-temp);
    auto temp3 =  1/log(temp2);
    auto Z0s = 120*M_PI*M_PI * temp3;
    auto k0 = 2.0*M_PI / lambda;
    dcomp ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);
    auto l=L;
    auto temp4=(a-L)/2;
    auto xmin=temp4;
    auto xmax=temp4+l;
    double temp6 = 0;

    temp6 = CalcTemp(iter, RungeVal, xmax, xmin, temp6, m, a, L, intval);

    auto Cma = temp6/xbol;
    dcomp temp9= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    dcomp Zap = 0.5 * Cma  * j * Z0s * temp9;
    dcomp Ztmp = Z0 + Zap;
    auto v1 = v0 * Zap/Ztmp;
    // auto Z1 = Z0 * Zap / Ztmp;
    // auto temp10 = pow((lambda*m/(2*a)),2);
    // auto temp11 = pow((lambda*n/(2*b)),2);
    // dcomp tmp_zg = dcomp(1.0,0.0)-dcomp(temp11,0.0)-dcomp(temp10,0.0);
    // dcomp Zg = Z0 / ( sqrt(tmp_zg));
    // auto kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    // auto temp12 = kg * p;
    // auto v2 = v1 / (cos(temp12) + j*(Z1/Zg)*sin(temp12)*sin((m*M_PI*p)/a));
    // auto temp13 = tan(temp12);
    // dcomp Z2 = (Z1+j*Zg*temp13) / (dcomp(1.0,0.0)+j*Z1/Zg*temp13);
    // dcomp Z3 = j*Zg*tan(kg*(d-p));
    // auto vp = (v2*Z3) / (Z2+Z3);

    // Преобразование в точку A
    dcomp z1=Z0*Zap/(Z0+Zap);
    dcomp vp1(0,0);

    for (int mm=1; mm<=m; mm++){
        for (int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=Z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));
            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vp1=vp+vp1;

        }
    }

    return -20*log10(abs(dcomp(2.0,0.0)*vp1/v0));
}

double Calculation_Funcs::calcsomePoadPlus(int *iter,double freq, double t, double w, double b, double L, double a, double d, double p, double xbol, double ybol, double n, double m, double sigma, double intval, bool RungeVal)
{
    auto c0 = 299792458;
    auto mue_r =  1.000023;
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    auto z0 = 120*M_PI;
    dcomp Z0(z0,0.0);
    dcomp j(0.0,1.0);
    auto we = w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI));
    auto kc = we / b;
    auto temp = pow((1-kc*kc),0.25);
    auto temp2 = (2+2*temp) / (1-temp);
    auto temp3 =  1/log(temp2);
    auto Z0s = 120*M_PI*M_PI* temp3;
    auto k0 = 2.0*M_PI / lambda;
    auto ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);
    auto l=L;
    auto temp4=(a-L)/2;
    auto temp6=(b-w)/2;
    auto xmin=temp4;
    auto xmax=temp4+l;
    auto ymin=temp6;
    auto ymax=temp6+w;
    double temp8 = 0,temp9 = 0;
    ++(*iter);
    temp8 = CalcTemp(iter, RungeVal, xmax, xmin, temp8, m, a, L, intval);

    auto Cm_x=temp8/xbol;

    temp9 = CalcTemp(iter, RungeVal, ymax, ymin, temp9, m, a, L, intval);

    auto Cm_y=temp9/ybol;
    auto Cma = Cm_x+Cm_y;
    auto temp10= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    auto Zap = Cma / 2 * j * Z0s * temp10;
    auto Ztmp = Z0 + Zap;
    auto v1 = v0 * Zap/Ztmp;
    auto Z1 = Z0 * Zap / Ztmp;
    auto temp11 = pow((lambda*m/(2*a)),2);
    auto temp12 = pow((lambda*n/(2*b)),2);
    auto tmp_zg = dcomp(1.0,0.0)-dcomp(temp12,0.0)-dcomp(temp11,0.0);
    auto Zg = Z0 / ( sqrt(tmp_zg));
    auto kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    auto temp13 = kg * p;
    auto v2 = v1 / (cos(temp13) + j*(Z1/Zg)*sin(temp13)*sin((m*M_PI*p)/a));
    auto temp14 = tan(temp13);
    auto Z2 = (Z1+j*Zg*temp14) / (dcomp(1.0,0.0)+j*Z1/Zg*temp14);
    auto Z3 = j*Zg*tan(kg*(d-p));
    auto vp = (v2*Z3) / (Z2+Z3);
    auto SE2 = 20*log10(abs(v0/(dcomp(2.0,0.0)*vp)));
    return SE2;
}

double Calculation_Funcs::calcsomePoadMultiple(int *iter,double freq, double t, double w, double b, double L, double a, double d, double p, double xbol, double ybol, double n, double m, double sigma, double intval, bool RungeVal)
{
    ++(*iter);
    auto c0 = 299792458;
    auto mue_r =  1.000023;
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    auto z0 = 120*M_PI;
    dcomp Z0(z0,0.0);
    dcomp j(0.0,1.0);
    auto we = w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI));
    auto kc = we / b;
    auto temp = pow((1-kc*kc),0.25);
    auto temp2 = (2+2*temp) / (1-temp);
    auto temp3 =  1/log(temp2);
    auto Z0s = 120*M_PI*M_PI* temp3;
    auto k0 = 2.0*M_PI / lambda;
    auto ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);
    auto l=L;
    auto temp4=(a-L)/2;
    auto temp6=(b-w)/2;
    auto xmin=temp4;
    auto xmax=temp4+l;
    auto ymin=temp6;
    auto ymax=temp6+w;
    double temp8 = 0,temp9 = 0;

    temp8 = CalcTemp(iter,RungeVal, xmax, xmin, temp8, m, a, L, intval);

    auto Cm_x=temp8/xbol;

    temp9 = CalcTemp(iter, RungeVal, ymax, ymin, temp9, m, a, L, intval);

    auto Cm_y=temp9/ybol;
    auto Cma = Cm_x*Cm_y;
    auto temp10= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    auto Zap = Cma / 2 * j * Z0s * temp10;
    auto Ztmp = Z0 + Zap;
    auto v1 = v0 * Zap/Ztmp;
    auto Z1 = Z0 * Zap / Ztmp;
    auto temp11 = pow((lambda*m/(2*a)),2);
    auto temp12 = pow((lambda*n/(2*b)),2);
    auto tmp_zg = dcomp(1.0,0.0)-dcomp(temp12,0.0)-dcomp(temp11,0.0);
    auto Zg = Z0 / ( sqrt(tmp_zg));
    auto kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    auto temp13 = kg * p;
    auto v2 = v1 / (cos(temp13) + j*(Z1/Zg)*sin(temp13)*sin((m*M_PI*p)/a));
    auto temp14 = tan(temp13);
    auto Z2 = (Z1+j*Zg*temp14) / (dcomp(1.0,0.0)+j*Z1/Zg*temp14);
    auto Z3 = j*Zg*tan(kg*(d-p));
    auto vp = (v2*Z3) / (Z2+Z3);
    auto SE1 = 20*log10(abs(v0/(dcomp(2.0,0.0)*vp)));
    return SE1;
}

double Calculation_Funcs::calcsomeAKC(int *iter,double freq,double t,double w,double b,double L,double a, double d,double p,double xbol,double ybol,double sigma, double m, double n)
{
    ++(*iter);
    auto c0 = 299792458;
    auto mue_r =  1.000023;
    dcomp j(0.0,1.0);
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    auto z0 = 120*M_PI;
    dcomp Z0(z0,0.0);
    auto we = w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI));
    auto kc = we / b;
    auto temp = pow((1-kc*kc),0.25);
    auto temp2 = (2+2*temp) / (1-temp);
    auto temp3 = M_PI * 1.0 / log(temp2);
    auto Z0s = 120.0 * M_PI * temp3;
    auto k0 = 2.0*M_PI / lambda;
    auto ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);
    auto beta = (xbol-ybol)/L-0.5;
    auto temp4 =(cos(M_PI*((ybol/a)+ beta))) / (a - L);
    dcomp temp5 = sqrt(dcomp(cos(pow((M_PI*((ybol/a)- beta)),2)),0.0)) / (a + L);
    auto temp6 = (a*(pow(L,2)))/(M_PI*xbol*ybol);
    auto temp7 = sqrt(sin(pow((M_PI*w*(a-L)/(2*a*L)),2)));
    dcomp Cma = (temp4-temp5)*temp6*temp7;
    dcomp temp8= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    dcomp Zap = Cma / dcomp(2.0,0.0) * j * Z0s * temp8;
    dcomp Ztmp = Z0 + Zap;
    dcomp v1 = v0 * Zap/Ztmp;
    // dcomp Z1 = Z0 * Zap / Ztmp;
    // auto temp9 = pow((lambda/(2*a)),2);
    // auto tmp_zg = dcomp(1.0,0.0)-dcomp(temp9,0.0);
    // dcomp Zg = Z0 / ( sqrt(tmp_zg));
    // dcomp kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    // dcomp temp10 = kg * p;
    // dcomp v2 = v1 / (cos(temp10) + j*Z1/Zg*sin(temp10));
    // dcomp temp11 = tan(temp10);
    // dcomp Z2 = (Z1+j*Zg*temp11) / (dcomp(1.0,0.0)+j*Z1/Zg*temp11);
    // dcomp Z3 = Zg*(ZL+j*Zg*tan(kg*(d-p)))/(Zg+j*ZL*tan(kg*(d-p)));
    // dcomp vp = (v2*Z3) / (Z2+Z3);

    // Преобразование в точку A
    dcomp z1=Z0*Zap/(Z0+Zap);
    dcomp vp1(0,0);

    for (int mm=1; mm<=m; mm++){
        for (int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=Z0/sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            dcomp kg=k0*sqrt(dcomp(1,0)-pow((lambda*mm/(dcomp(2,0)*a)),2)-pow((lambda*nn/(dcomp(2,0)*b)),2));
            ++(*iter);

            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1,0)+j*(z1/zg)*tan(kg*p));

            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vp1=vp+vp1;

        }
    }

    return -20*log10(abs(dcomp(2.0,0.0)*vp1/v0));
}
//30.08.2018 Komnantov calculation method
double Calculation_Funcs::calcMethod2(double a, double d, double b, double p, double fm, double mnoj, double S11, double m, double n){
    // IVANOV 1
    double c0 = 299792458.0;
    double f;
    if (fm == 0.0) f=0.000001;
    else f = fm * mnoj;
    dcomp j(0.0,1.0);
    double lambda = c0/f;
    auto nu0 = (120.0 * M_PI);

    //Волновое число
    double k0 = 2.0*M_PI / lambda;
    dcomp z0(50.0,0.0);
    dcomp v0(1.0,0.0);

    // Сопротивление стенки с апертурой
    dcomp z=dcomp(2.0,0.0)*z0*dcomp(S11/(1.0-S11),0.0);
    dcomp zgg=nu0/sqrt(dcomp(1.0,0.0)-pow((lambda/(dcomp(2.0,0.0)*a)),2));
    dcomp kgg=k0*sqrt(dcomp(1.0,0.0)-pow((lambda/(dcomp(2.0,0.0)*a)),2));

    std::cout << z.real() << z.imag() << std::endl << zgg.real() << zgg.imag() << std::endl << kgg.real() << kgg.imag() << std::endl;
    dcomp vpp(0.0,0.0); // суммарное напряжение в точке наблюдения

    // Эти циклы для учета высших мод TEmn (mm,nn за номер моды):
    for (int mm=1; mm<=m; mm++){
        for(int nn=0; nn<=n; nn++){
            // Характеристический импеданс и постоянная распространения в корпусе
            dcomp zg=nu0/sqrt(dcomp(1.0,0.0)-pow((lambda*mm/(2.0*a)),2)-pow((lambda*nn/(2.0*b)),2));
            dcomp kg=k0*sqrt(dcomp(1.0,0.0)-pow((lambda*mm/(2.0*a)),2)-pow((lambda*nn/(2.0*b)),2));
            dcomp zsc=j*zg*tan(kg*d);
            dcomp zap=z*zsc/(zsc-z);

            // Преобразование в точку A
            dcomp v1=v0*zap/(nu0+zap);
            dcomp z1=nu0*zap/(nu0+zap);
            // Преобразование в точку P
            dcomp v2=v1/(cos(kg*p)+j*(z1/zg)*sin(kg*p));
            dcomp z2=(z1+j*zg*tan(kg*p))/(dcomp(1.0,0.0)+j*(z1/zg)*tan(kg*p));

            // Нагрузка
            dcomp z3=j*zg*tan(kg*(d-p));

            // Напряжение в точке P
            dcomp vp=v2*z3/(z2+z3);
            vpp=vp+vpp;
        }
    }

    return -20.0*log10(abs(dcomp(2.0,0.0)*vpp/v0));

}
//30.08.2018 Komnantov calculation method
double Calculation_Funcs::calcMethod(double a, double d, double b, double p, double fm, double mnoj, double S11){
    auto c0 = 299792458.0;
    auto f = fm * mnoj;
    dcomp j(0.0,1.0);
    auto lambda = c0/f;

    //Волновое число
    auto k0 = 2.0*M_PI / lambda;
    dcomp kg = k0 * sqrt(1.0-pow(lambda/(dcomp(2.0,0.0)*a),2));
    // double Z0 = 50.0;
    auto nu0 = 120.0 * M_PI;
    dcomp Zg = (nu0)/sqrt(1.0-pow(lambda/(dcomp(2.0,0.0)*a),2));
    dcomp Zsc = j* Zg * tan(kg*(d-p)); // 3.7204066053e-1i
    dcomp temp1 = Zsc - (Zsc+ dcomp(2.0,0.0)*nu0)*S11; //-2.5901843574e0+3.7331874621e-1i
    dcomp temp2 = dcomp(2.0,0.0) * Zsc * nu0 * S11; // 9.6365389922e-1i
    dcomp chis1 = Zg * temp2 * temp1 * (cos(kg*p)-tan(kg*p)*sin(kg*p))+ j*sin(kg*p)*(pow(temp2,2)+pow(Zg,2)*pow(temp1,2));
    dcomp znam1 = dcomp(4.0,0.0) * j * Zg * tan(kg*(a-p))*Zsc*S11*(Zg*temp1+j*tan(kg*p)*(temp2));
    dcomp slag1 = chis1/znam1;
    dcomp slag2 = (Zg*temp1*cos(kg*p)+j*sin(kg*p)*(temp2))/(dcomp(4.0,0.0)*Zsc*Zg*S11);
    dcomp x = slag1+slag2; // 3.6645531585e2+2.0192008696e2i

    return 20.0*log10(abs(x));
}
double Calculation_Funcs::calcsomeAKCintegral(int *iter,double freq, double t, double w, double b, double L, double a, double d, double p, double xbol, double ybol, double n, double m, double sigma, double intval, bool RungeVal)
{
    auto c0 = 299792458; // константы подключить в общую структуру проекта
    auto mue_r =  1.000023; //------******--------

    dcomp j(0.0,1.0);
    auto lambda = c0/freq;
    dcomp v0(1.0,0.0);
    auto z0 = 120*M_PI;
    dcomp Z0(z0,0.0);
    ++(*iter);

    //Эффективная штрина
    auto we = w - ((5*t*(1+log(4*M_PI*w/t))) / (4 *M_PI));

    //Сопротивление линии передачи
    auto kc = we / b;
    auto temp = pow((1-kc*kc),0.25); // fp_type temp = pow((1 - kc*kc),fp_type(0.25));
    auto temp2 = (2+2*temp) / (1-temp); // fp_type temp_temp = 2*(1+temp) / (1-temp);
    auto temp3 = M_PI * 1.0 / log(temp2); // fp_type F = PI * 1.0 / log(temp_temp);
    auto Z0s = 120.0 * M_PI * temp3;

    //Волновое число
    auto k0 = 2.0*M_PI / lambda;

    auto ZL=dcomp(1.0,1.0)*pow(((M_PI*freq*mue_r)/sigma),0.5);

    auto l=L;
    auto temp4=(a-L)/2;
    auto temp6=(b-w)/2;
    auto xmin=temp4;
    auto xmax=temp4+l;   // пределы интегрирования по иксу
    auto ymin=temp6;
    auto ymax=temp6+w;   // пределы интегрирования по игрику
    double temp8 = 0,temp9 = 0;

    temp8 = CalcTemp(iter, RungeVal, xmax, xmin, temp8, m, a, L, intval);

    auto Cm_x=temp8/xbol;

    temp9 = CalcTemp(iter, RungeVal, ymax, ymin, temp9, m, a, L, intval);

    auto Cm_y=temp9/ybol;
    auto Cma = Cm_x*Cm_y;

    auto temp88= (ZL+j*Z0s*tan(k0*L/2)) / (Z0s+j*ZL*tan(k0*L/2));
    auto Zap = Cma / 2 * j * Z0s * temp88;
    auto Ztmp = Z0 + Zap;
    auto v1 = v0 * Zap/Ztmp;
    auto Z1 = Z0 * Zap / Ztmp;

    auto temp99 = pow((lambda/(2*a)),2);
    auto tmp_zg = dcomp(1.0,0.0)-dcomp(temp99,0.0);
    auto Zg = Z0 / ( sqrt(tmp_zg));
    auto kg = dcomp(k0,0.0)*sqrt(tmp_zg);
    auto temp10 = kg * p;
    auto v2 = v1 / (cos(temp10) + j*Z1/Zg*sin(temp10));
    auto temp11 = tan(temp10);
    auto Z2 = (Z1+j*Zg*temp11) / (dcomp(1.0,0.0)+j*Z1/Zg*temp11);
    auto Z3 = Zg*(ZL+j*Zg*tan(kg*(d-p)))/(Zg+j*ZL*tan(kg*(d-p)));
    auto vp = (v2*Z3) / (Z2+Z3);
    auto SE6 = 20*log10(abs(v0/(dcomp(2.0,0.0)*vp)));
    return SE6;
}
