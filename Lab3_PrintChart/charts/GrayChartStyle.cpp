#include "charts/GrayChartStyle.h"

#include <QColor>
#include <QtCharts/QChart>
#include <algorithm>

QT_CHARTS_USE_NAMESPACE

namespace
{
// Диапазон яркости серого: от тёмного до светлого, чтобы соседние секторы
// различались на чёрно-белой печати.
constexpr int kGrayMin = 60;
constexpr int kGrayMax = 230;
} // namespace

QColor GrayChartStyle::colorFor(int index, int total) const
{
    const int span = kGrayMax - kGrayMin;
    const int gray = kGrayMin + (index * span) / std::max(total - 1, 1);
    return QColor(gray, gray, gray);
}

void GrayChartStyle::apply(QChart *chart) const
{
    chart->setTheme(QChart::ChartThemeLight);
    chart->setBackgroundBrush(Qt::white);
}
