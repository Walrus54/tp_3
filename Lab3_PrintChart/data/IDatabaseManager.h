#ifndef IDATABASEMANAGER_H
#define IDATABASEMANAGER_H

#include <QString>
#include <QStringList>

#include "data/ChartData.h"

/**
 * @brief Абстракция доступа к БД.
 *
 * Прячет работу с драйвером и соединениями за интерфейсом, чтобы читатель
 * (SqliteDataReader) не зависел напрямую от QtSql и допускал подмену движка БД
 * или мока в тестах. Имена колонок задаёт вызывающий — менеджер не знает схемы.
 */
class IDatabaseManager
{
public:
    virtual ~IDatabaseManager() = default;

    /**
     * @brief Список таблиц в файле БД.
     * @param dbPath путь к файлу БД.
     */
    virtual QStringList tables(const QString &dbPath) const = 0;

    /**
     * @brief Прочитать ряд из двух колонок таблицы.
     * @param dbPath путь к файлу БД.
     * @param table имя таблицы.
     * @param labelColumn колонка метки (ось X).
     * @param valueColumn колонка значения.
     * @return точки ряда; при ошибке — пустой вектор.
     */
    virtual QVector<ChartPoint> readSeries(const QString &dbPath,
                                           const QString &table,
                                           const QString &labelColumn,
                                           const QString &valueColumn) const = 0;
};

#endif // IDATABASEMANAGER_H
