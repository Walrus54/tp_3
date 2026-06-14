#ifndef PIECHARTBUILDER_H
#define PIECHARTBUILDER_H

#include "charts/IChartBuilder.h"

/**
 * @brief Строитель круговой диаграммы (PieChart).
 */
class PieChartBuilder : public IChartBuilder
{
public:
    QtCharts::QChart *build(const Series &data, const IPalette &palette) const override;
};

#endif // PIECHARTBUILDER_H
