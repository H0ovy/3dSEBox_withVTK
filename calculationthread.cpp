#include "calculationthread.h"


using namespace std;

CalculationThread::CalculationThread(){}

double CalculationThread::GetCalculation(int i, double tempFreq, double pp)
{
    switch (i)
    {
        ///{
        // 1 аппертура
        case 0:
            return func_calcs.calcSomeRob(iterations, m_fMinVal, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, m_pVal, m_mVal, m_nVal);
        case 1:
            return func_calcs.calcSomeRob(iterations, tempFreq, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, pp, m_mVal, m_nVal);


        case 2:
            return func_calcs.calcsomeYongshi(iterations,m_fMinVal, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, m_pVal, m_xVal, m_yVal, m_nVal, m_mVal, m_sigmaVal, m_integralVal, m_RungeVal);
        case 3:
            return func_calcs.calcsomeYongshi(iterations,tempFreq, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, pp, m_xVal, m_yVal, m_nVal, m_mVal, m_sigmaVal, m_integralVal, m_RungeVal);
        ///}


        case 4:
            return func_calcs.calcsomePoad(iterations, m_fMinVal, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, m_pVal, m_xVal, m_nVal, m_mVal, m_sigmaVal, m_integralVal, m_RungeVal);
        case 5:
            return func_calcs.calcsomePoad(iterations, tempFreq, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, pp, m_xVal, m_nVal, m_mVal, m_sigmaVal, m_integralVal, m_RungeVal);

        case 6:
            return func_calcs.calcsomeAKC(iterations, m_fMinVal, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, m_pVal, m_xVal, m_yVal,  m_sigmaVal, m_mVal, m_nVal);
        case 7:
            return func_calcs.calcsomeAKC(iterations, tempFreq, m_tVal, m_wVal, m_bVal, m_lVal, m_aVal, m_dVal, pp, m_xVal, m_yVal, m_sigmaVal, m_mVal, m_nVal);

        case 8:
            return func_calcs.calcNIEetal(iterations, m_fMinVal, m_wVal, m_lVal, m_xVal, m_yVal, m_pVal, m_dVal, m_bVal, m_aVal, m_tVal, m_mVal, m_nVal);
        case 9:
            return func_calcs.calcNIEetal(iterations, tempFreq, m_wVal, m_lVal, m_xVal, m_yVal, pp, m_dVal, m_bVal, m_aVal, m_tVal, m_mVal, m_nVal);

        ///{
        // Массив
        case 10:
            return func_calcs.ren(iterations, m_fMinVal, m_aVal, m_bVal, m_pVal, m_dVal, m_tVal, m_wVal, m_lVal, m_napVal, m_mapVal, m_mVal, m_nVal);
        case 11:
            return func_calcs.ren(iterations, tempFreq, m_aVal, m_bVal, pp, m_dVal, m_tVal, m_wVal, m_lVal, m_napVal, m_mapVal, m_mVal, m_nVal);

        case 12:
            return func_calcs.Dehkhoda_2007(iterations, m_fMinVal, m_aVal, m_bVal, m_pVal, m_dVal, m_wVal, m_napVal, m_mapVal, m_mVal, m_nVal, m_dhVal, m_dvVal);
        case 13:
            return func_calcs.Dehkhoda_2007(iterations, tempFreq, m_aVal, m_bVal, pp, m_dVal, m_wVal, m_napVal, m_mapVal, m_mVal, m_nVal, m_dhVal, m_dvVal);

        case 14:
            return func_calcs.Nie_2017(iterations, m_fMinVal, m_aVal, m_bVal, m_pVal, m_dVal, m_tVal, m_wVal, m_napVal, m_mapVal, m_mVal, m_nVal, m_dhVal, m_dvVal);
        case 15:
            return func_calcs.Nie_2017(iterations, tempFreq, m_aVal, m_bVal, pp, m_dVal, m_tVal, m_wVal, m_napVal, m_mapVal, m_mVal, m_nVal, m_dhVal, m_dvVal);
        ///}

        ///{
        //  Цилиндр
        case 16:
            return func_calcs.WAMGetal(iterations, m_fMinVal, m_apVal, m_rVal, m_tVal, m_dVal, m_pVal);
        case 17:
            return func_calcs.WAMGetal(iterations, tempFreq, m_apVal, m_rVal, m_tVal, m_dVal, pp);
        ///}
    }
    return 0;
}

void CalculationThread::CalcThread(double tempValue, int K)
{
    double tempFreq = 0;
    double dfreq,ival=0;
    int a = 0;
    iterations = &a;
    double progres_val = 100 / (m_nPointsVal * (m_dVal - m_pVal) / perc_step);
    bool abort = false;
    size = 0;
    // tempValue = GetCalculation(K, 0, 0);
    dfreq=(m_fMaxVal - m_fMinVal) / m_nPointsVal;
    double pp = m_pVal;

        for(int Z = (int)(m_pVal * 1000); Z <= (int)(m_dVal * 1000); Z += 1)
        {
            for(int i=0; i < m_nPointsVal; ++i)
            {
                    if (QThread::currentThread()->isInterruptionRequested())
                    {
                        abort = true;
                        break;
                    }
                    ival = ival + progres_val;
                    emit progress(ival);
                    tempFreq = m_fMinVal + dfreq * i;
                    tempValue = GetCalculation(K + 1, tempFreq, pp);
                    if (isnan(tempValue))
                        tempValue=0;

                    mItems.append({ tempFreq, tempValue, pp});
                    size = size + 1;
            }
            if (abort)
            {
                mItems.remove(mItems.size() - size, size);
                break;
            }
            pp += perc_step;
            Z = (pp * 1000);
        }
    emit iterCount(*iterations);
    iterations = &a;
}

void CalculationThread::run()
{
    double tempValue = 0;
    int a = 0;
    size = 0;
    iterations = &a;
    QElapsedTimer timer;
    mItems.clear();

    if (!toShow.empty())
    {
        surfaceModelItem tmp;

        QVector<surfaceModelItem>::const_iterator it, end(toShow.end());
        for (it = toShow.begin(); it != end; ++it)
        {
            tmp.x = it->x;
            tmp.y = it->y;
            tmp.z = it->z;
            mItems.append(tmp);
        }
        toShow.clear();
    }
    else if(m_fileBool == true)
    {
        QFile file(m_file);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        int multiVal;
        if (modS == 0) multiVal = 1;
        else if (modS == 1) multiVal = 1000000;
        else if (modS == 2) multiVal = 1000000000;
        for(double pp = m_pVal; pp <= m_dVal; pp += perc_step)
        {
            QTextStream in(&file);
            in.seek(0);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if(line.contains("#")) continue;
                QStringList list1 = line.split('\t');
                tempValue = func_calcs.calcMethod2(m_aVal, m_dVal, m_bVal,pp, list1.at(0).toDouble(), multiVal, list1.at(1).toDouble(), m_mVal, m_nVal);
                mItems.append({ list1.at(0).toDouble()*multiVal, tempValue, pp});
                size = size + 1;
             }
        }
    }
    else
    {
        switch (mod)
        {
        case 0:
            timer.start();
            CalcThread(tempValue, mod);
            emit time(timer.elapsed());
            qDebug() << "The calcSomeRob took" << timer.elapsed() << "milliseconds";
            break;
        case 1:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomeYongshi took " << timer.elapsed() << "milliseconds";
            break;
        case 2:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomePoad took " << timer.elapsed() << "milliseconds";
            break;
        case 3:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsome KOMNATNOV took " << timer.elapsed() << "milliseconds";
            break;
        case 4:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomeNie took " << timer.elapsed() << "milliseconds";
            break;
        case 5:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomeRen took " << timer.elapsed() << "milliseconds";
            break;
        case 6:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomeDehkoda took " << timer.elapsed() << "milliseconds";
            break;
        case 7:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            break;
        case 8:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomeWAMG took " << timer.elapsed() << "milliseconds";
            break;
        case 10:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomePoadPlus took " << timer.elapsed() << "milliseconds";
            break;
        case 11:
            timer.start();
            CalcThread(tempValue, mod + mod);
            emit time(timer.elapsed());
            qDebug() << "The calcsomePoadMulti took " << timer.elapsed() << "milliseconds";
            break;
        }
    }
    emit GUI(mItems);
    emit progress(0);
}
