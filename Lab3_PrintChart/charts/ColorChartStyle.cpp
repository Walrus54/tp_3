#include "charts/ColorChartStyle.h"

#include <QColor>
#include <QVector>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

namespace
{
// Качественная палитра (Tableau 10 + доп.). Цвета берутся по индексу циклически.
const QVector<QColor> kPalette = {
    QColor(0x4e, 0x79, 0xa7), QColor(0xf2, 0x8e, 0x2b), QColor(0xe1, 0x57, 0x59),
    QColor(0x76, 0xb7, 0xb2), QColor(0x59, 0xa1, 0x4f), QColor(0xed, 0xc9, 0x48),
    QColor(0xb0, 0x7a, 0xa1), QColor(0xff, 0x9d, 0xa7), QColor(0x9c, 0x75, 0x5f),
    QColor(0xba, 0xb0, 0xac), QColor(0x43, 0x9a, 0xcc),
};
} // namespace

QColor ColorChartStyle::colorFor(int index, int total) const
{
    Q_UNUSED(total);
    return kPalette.at(index % kPalette.size());
}

void ColorChartStyle::apply(QChart *chart) const
{
    // Раскраска серий идёт через colorFor() в билдерах; здесь только общая тема.
    chart->setTheme(QChart::ChartThemeLight);
}
