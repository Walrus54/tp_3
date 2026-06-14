#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QString>
#include <QVector>

/**
 * @brief Одна точка ряда: пара [метка времени, значение].
 *
 * Единый формат, к которому любой источник (SQLite, JSON, ...) приводит свои
 * данные, чтобы билдеры графиков не зависели от формата исходного файла.
 */
struct ChartPoint
{
    QString label; ///< метка по оси X (дата/время в виде строки)
    double value;  ///< числовое значение
};

/**
 * @brief Временной ряд: имя и набор точек.
 */
struct Series
{
    QString name;             ///< имя ряда (таблица БД / имя ряда JSON)
    QVector<ChartPoint> points;
};

#endif // CHARTDATA_H
