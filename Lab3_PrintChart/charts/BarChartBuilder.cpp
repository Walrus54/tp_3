#include "charts/BarChartBuilder.h"

#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <algorithm>

#include "data/Aggregator.h"

QT_CHARTS_USE_NAMESPACE

namespace
{
// Bar-чарт использует один QBarSet — палитра выдаёт единственный цвет.
constexpr int kSingleSetIndex = 0;
constexpr int kSingleSetTotal = 1;
// Запас по оси Y сверху/снизу относительно размаха значений (15%).
constexpr double kAxisYPaddingRatio = 0.15;
// Число делений оси Y.
constexpr int kAxisYTickCount = 6;
} // namespace

QChart *BarChartBuilder::build(const Series &raw, const IPalette &palette) const
{
    // При большом числе точек агрегируем по месяцу — иначе столбцы нечитаемы.
    const Series data = Aggregator::aggregateIfLarge(raw);

    QBarSet *set = new QBarSet(data.name);
    QStringList categories;
    double minVal = 0.0, maxVal = 0.0;
    for (int i = 0; i < data.points.size(); ++i) {
        const double v = data.points.at(i).value;
        *set << v;
        categories << data.points.at(i).label;
        if (i == 0) {
            minVal = maxVal = v;
        } else {
            minVal = std::min(minVal, v);
            maxVal = std::max(maxVal, v);
        }
    }
    set->setColor(palette.colorFor(kSingleSetIndex, kSingleSetTotal));

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(data.name);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    const double padding = (maxVal - minVal) * kAxisYPaddingRatio;
    axisY->setRange(std::min(0.0, minVal - padding), maxVal + padding);
    axisY->setTickCount(kAxisYTickCount);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(false);
    return chart;
}
