#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include "data/ChartData.h"

/**
 * @brief Агрегация длинных рядов по префиксу метки времени (среднее в группе).
 *
 * Графики на десятки тысяч точек нечитаемы — сводим точки к месяцам/годам,
 * группируя по префиксу строки-метки фиксированной длины.
 */
namespace Aggregator
{
/// Порог, выше которого включается агрегация.
constexpr int kThreshold = 50;
/// Длина ключа по умолчанию: 7 символов -> "MM.YYYY" (группировка по месяцам).
constexpr int kMonthKeyLen = 7;
/// Длина ключа для группировки по годам: 4 символа -> "YYYY".
constexpr int kYearKeyLen = 4;
/// Длина префикса "DD." в формате "DD.MM.YYYY" — пропускается при группировке.
constexpr int kDayPrefixLen = 3;

/**
 * @brief Сгруппировать точки по префиксу метки и усреднить значения в группе.
 * @param src исходный ряд.
 * @param keyLen длина префикса-ключа (kMonthKeyLen / kYearKeyLen).
 * @return ряд из усреднённых точек в порядке первого появления ключей.
 */
Series aggregate(const Series &src, int keyLen = kMonthKeyLen);

/**
 * @brief Агрегировать только если точек больше kThreshold, иначе вернуть как есть.
 */
Series aggregateIfLarge(const Series &src);
} // namespace Aggregator

#endif // AGGREGATOR_H
