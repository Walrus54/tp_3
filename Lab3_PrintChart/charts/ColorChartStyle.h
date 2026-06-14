#ifndef COLORCHARTSTYLE_H
#define COLORCHARTSTYLE_H

#include "charts/IChartStyle.h"

/**
 * @brief Цветной стиль: качественная палитра + светлая тема.
 */
class ColorChartStyle : public IChartStyle
{
public:
    QColor colorFor(int index, int total) const override;
    void apply(QtCharts::QChart *chart) const override;
};

#endif // COLORCHARTSTYLE_H
