#ifndef GRAYCHARTSTYLE_H
#define GRAYCHARTSTYLE_H

#include "charts/IChartStyle.h"

/**
 * @brief Чёрно-белый стиль: равномерный градиент серого + белый фон.
 */
class GrayChartStyle : public IChartStyle
{
public:
    QColor colorFor(int index, int total) const override;
    void apply(QtCharts::QChart *chart) const override;
};

#endif // GRAYCHARTSTYLE_H
