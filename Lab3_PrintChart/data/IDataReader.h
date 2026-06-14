#ifndef IDATAREADER_H
#define IDATAREADER_H

#include <QStringList>

#include "data/ChartData.h"

/**
 * @brief Абстракция источника данных.
 *
 * Новый формат файла -> новая реализация интерфейса. UI и реестр зависят только
 * от IDataReader и не знают о конкретных форматах (SQLite, JSON, ...).
 */
class IDataReader
{
public:
    virtual ~IDataReader() = default;

    /**
     * @brief Расширения файлов, которые умеет читать читатель.
     * @return список в нижнем регистре, без точки (например {"json"}).
     */
    virtual QStringList extensions() const = 0;

    /**
     * @brief Подходит ли читатель для данного файла (по расширению).
     * @param filePath путь к файлу.
     */
    virtual bool canRead(const QString &filePath) const = 0;

    /**
     * @brief Перечислить под-источники внутри файла (например, таблицы БД).
     * @param filePath путь к файлу.
     * @return имена под-источников; для одиночных форматов — пустой список.
     */
    virtual QStringList listSubSources(const QString &filePath) const = 0;

    /**
     * @brief Прочитать данные.
     * @param source "путь" либо "путь|под-источник" (например "db.sqlite|TABLE").
     * @return ряд; при ошибке — ряд без точек.
     */
    virtual Series read(const QString &source) = 0;
};

#endif // IDATAREADER_H
