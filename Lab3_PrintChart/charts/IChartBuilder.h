#ifndef ICHARTBUILDER_H
#define ICHARTBUILDER_H

#include <QString>

#include "charts/IPalette.h"
#include "data/ChartData.h"

// Опережающее объявление: интерфейс не тянет тяжёлый <QtCharts/QChart>
// в потребителей — полный тип нужен только реализациям (в .cpp).
QT_BEGIN_NAMESPACE
namespace QtCharts { class QChart; }
QT_END_NAMESPACE

/**
 * @brief Абстракция строителя графика.
 *
 * Новый вид графика -> новая реализация, добавляется в композиционный корень
 * без правки остального кода.
 */
class IChartBuilder
{
public:
    virtual ~IChartBuilder() = default;

    /**
     * @brief Построить график по данным.
     * @param data исходный ряд.
     * @param palette палитра для раскраски элементов.
     * @return новый QChart; владение передаётся вызывающему.
     */
    virtual QtCharts::QChart *build(const Series &data, const IPalette &palette) const = 0;
};

#endif // ICHARTBUILDER_H
