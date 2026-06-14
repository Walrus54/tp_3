#include "charts/PieChartBuilder.h"

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

#include "data/Aggregator.h"

QT_CHARTS_USE_NAMESPACE

namespace
{
// Максимум секторов, читаемых на круговой диаграмме; выше — группируем по годам.
constexpr int kMaxSlices = 24;
} // namespace

QChart *PieChartBuilder::build(const Series &raw, const IPalette &palette) const
{
    // Круговая диаграмма читаема лишь на немногих секторах — агрегируем сильнее.
    Series data = Aggregator::aggregateIfLarge(raw);
    // Если после месячной агрегации секторов всё ещё слишком много — по годам.
    if (data.points.size() > kMaxSlices)
        data = Aggregator::aggregate(raw, Aggregator::kYearKeyLen);

    QChart *chart = new QChart();
    chart->setTitle(data.name);

    QPieSeries *series = new QPieSeries();
    const int total = data.points.size();
    for (int i = 0; i < total; ++i) {
        QPieSlice *slice = series->append(data.points.at(i).label, data.points.at(i).value);
        slice->setBrush(palette.colorFor(i, total));
        slice->setLabelVisible(true);
    }
    chart->addSeries(series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    return chart;
}
