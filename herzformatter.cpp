#include "herzformatter.h"
#include <QtDataVisualization/QValue3DAxis>
#include <QtQml/QQmlExtensionPlugin>
#include <QtCore/QDebug>

//using namespace QtDataVisualization;

HerzFormatter::HerzFormatter(QObject *parent) :
    QValue3DAxisFormatter(parent)
{
}
HerzFormatter::~HerzFormatter()
{
}
QValue3DAxisFormatter *HerzFormatter::createNewInstance() const
{
    return new HerzFormatter();
}
void HerzFormatter::populateCopy(QValue3DAxisFormatter &copy) const
{
    QValue3DAxisFormatter::populateCopy(copy);
}
QString HerzFormatter::stringForValue(qreal value, const QString &format) const
{
    Q_UNUSED(format);
    QString result = QString::number(value)+ QObject::tr("");
    if(value >= 1000){
        result = QString::number(value/1000) + QObject::tr("×10³");
    }
    if(value>=1000000){
        result = QString::number(value/1000000) + QObject::tr("×10⁶");
    }
    if(value>=1000000000){
        result = QString::number(value/1000000000) + QObject::tr("×10⁹");
    }
    if(m_selectedLocale != "en_US") result.replace(".",",");
    qDebug() <<result;

    return result;
}
QString HerzFormatter::selectedLocale() const
{
    return m_selectedLocale;
}
void HerzFormatter::setSelectedLocale(const QString &locale)
{
    if (m_selectedLocale != locale) {
        m_selectedLocale = locale;
        markDirty(true); // Necessary to regenerate already visible selection label
        emit selectedLocaleChanged(locale);
    }
}
