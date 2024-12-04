#ifndef CALCULATIONTHREAD_H
#define CALCULATIONTHREAD_H

#include <QThread>
#include "robcalculation.h"
#include "QDate"
#include <QTimer>
#include "QThread"
#include "QDebug"
#include "robcalculation.h"
#include <QElapsedTimer>
#include <cmath>
#include <omp.h>
#include <mutex>
#include <QFile>

struct surfaceModelItem
{
    double x,y,z;
};

class CalculationThread : public QThread
{
    Q_OBJECT
signals:
    void progress(double);
    void time(double);
    void iterCount(double);
    void GUI(QVector<surfaceModelItem>);

public:
    CalculationThread();
    CalculationThread(double fMinVal, double fMaxVal, double tVal, double  wVal, double bVal, double rVal, double hVal, double apVal,
                      double lVal, double aVal, double dVal, double pVal, double nPointsVal,
                      double xVal, double yVal, double napVal, double mapVal, double nVal, double mVal, double dvVal,
                      double dhVal, double sigmaVal, double integralVal, bool RungeVal, bool fileBool)
        : m_fMinVal(fMinVal), m_fMaxVal(fMaxVal), m_tVal(tVal), m_wVal(wVal), m_bVal(bVal), m_rVal(rVal), m_hVal(hVal), m_apVal(apVal),
        m_lVal(lVal), m_aVal(aVal), m_dVal(dVal), m_pVal(pVal), m_nPointsVal(nPointsVal),
        m_xVal(xVal), m_yVal(yVal), m_napVal(napVal), m_mapVal(mapVal), m_nVal(nVal), m_mVal(mVal), m_dvVal(dvVal), m_dhVal(dhVal),
        m_sigmaVal(sigmaVal), m_integralVal(integralVal), m_RungeVal(RungeVal), m_fileBool(fileBool) {}

    void CalcThread(double tempValue, int K);
    double GetCalculation(int i, double tempFreq, double pp);
    void run();

    std::thread m_thread;
    robCalculation rob_calcs;
    int size = 0;
    QVector<surfaceModelItem> toShow;
    QVector<surfaceModelItem> mItems;
    int mod = 0;
    int modS = 0;
    int *iterations;
    QString m_file;
    double m_fMinVal, m_fMaxVal, m_tVal, m_wVal, m_bVal, m_rVal, m_hVal, m_apVal,
    m_lVal, m_aVal, m_dVal, m_pVal, m_nPointsVal,
    m_xVal, m_yVal, m_napVal, m_mapVal, m_nVal, m_mVal, m_dvVal, m_dhVal, m_sigmaVal, m_integralVal, perc_step;
    bool m_RungeVal, m_fileBool;
};

#endif // CALCULATIONTHREAD_H
