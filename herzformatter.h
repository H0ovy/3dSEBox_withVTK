#ifndef HERZFORMATTER_H
#define HERZFORMATTER_H

#include <QtDataVisualization/QValue3DAxisFormatter>
#include <QtCore/QDateTime>
#include <QtCore/QVector>

class HerzFormatter: public QValue3DAxisFormatter
{
    Q_OBJECT
    Q_PROPERTY(QString selectedLocale READ selectedLocale WRITE setSelectedLocale NOTIFY selectedLocaleChanged)
public:
    QString selectedLocale() const;
    explicit HerzFormatter(QObject *parent = 0);
    virtual ~HerzFormatter();
    virtual QValue3DAxisFormatter *createNewInstance() const;
    virtual void populateCopy(QValue3DAxisFormatter &copy) const;
    //virtual void recalculate(){};
    virtual QString stringForValue(qreal value, const QString &format) const;
public slots:
    void setSelectedLocale(const QString &locale);
signals:
    void selectedLocaleChanged(const QString &locale);
private:
    QString m_selectedLocale = "RU";
};

#endif // HERZFORMATTER_H
