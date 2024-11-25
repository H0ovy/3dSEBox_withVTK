#ifndef CALCULATIONTHREAD_H
#define CALCULATIONTHREAD_H

#include <QThread>
//#include "surfacemodellist.h"
#include "robcalculation.h"

class CalculationThread : public QThread
{
    Q_OBJECT
signals:
    //void progress(double);
    //void time(double);
    //void iterCount(double);
    //void GUI(QVector<surfaceModelItem>);
public:
    //void run();
public:
    //void CalcThread(double tempValue, int K/*, ptrfc*/);
    double GetCalculation(int i, double, double);
public:
    std::thread m_thread;
    //I created this variable to avoid calls on static functions
    robCalculation rob_calcs;
    int size = 0; //sauvegarde la taille du dernier graphic créer
    //QVector<surfaceModelItem> toShow;
    //QVector<surfaceModelItem> mItems;
    int mod = 0;
    int modS = 0;
    int *iterations;
    QString m_file;
    double m_fMinVal, m_fMaxVal, m_tVal, m_wVal, m_bVal, m_rVal, m_hVal, m_apVal,
    m_lVal, m_aVal, m_dVal, m_pVal,m_nPointsVal, perc_step,
    m_xVal, m_yVal, m_napVal, m_mapVal, m_nVal, m_mVal, m_dvVal, m_dhVal, m_sigmaVal, m_integralVal; bool m_RungeVal, m_fileBool;
};

#endif // CALCULATIONTHREAD_H
