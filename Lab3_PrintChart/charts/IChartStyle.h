#ifndef ICHARTSTYLE_H
#define ICHARTSTYLE_H

#include <QtGlobal>

#include "charts/IPalette.h"

QT_BEGIN_NAMESPACE
namespace QtCharts { class QChart; }
QT_END_NAMESPACE

/**
 * @brief Стиль графика: палитра (IPalette) + общие настройки графика.
 *
 * Покраска отдельных серий — обязанность билдеров через палитру; стилю не нужно
 * знать о конкретных типах series, он задаёт лишь тему/фон/легенду.
 */
class IChartStyle : public IPalette
{
public:
    /**
     * @brief Применить к графику общие настройки (тема, фон, легенда).
     * @param chart график для настройки.
     */
    virtual void apply(QtCharts::QChart *chart) const = 0;
};

#endif // ICHARTSTYLE_H
