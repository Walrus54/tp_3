#ifndef BARCHARTBUILDER_H
#define BARCHARTBUILDER_H

#include "charts/IChartBuilder.h"

/**
 * @brief Строитель столбчатой диаграммы (BarChart).
 */
class BarChartBuilder : public IChartBuilder
{
public:
    QtCharts::QChart *build(const Series &data, const IPalette &palette) const override;
};

#endif // BARCHARTBUILDER_H
