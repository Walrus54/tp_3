#include "data/SqliteDataReader.h"

#include <QFileInfo>
#include <utility>

namespace
{
const QStringList kExtensions = {"sqlite", "db", "sqlite3"};
const QString kLabelColumn = "Time";  // колонка метки времени (ось X)
const QString kValueColumn = "Value"; // колонка числового значения

// Разбор source "путь|таблица" на путь и (опционально) имя таблицы.
QPair<QString, QString> splitSource(const QString &source)
{
    const int pos = source.indexOf('|');
    if (pos < 0)
        return {source, QString()};
    return {source.left(pos), source.mid(pos + 1)};
}
} // namespace

SqliteDataReader::SqliteDataReader(std::shared_ptr<IDatabaseManager> db)
    : m_db(std::move(db))
{
}

QStringList SqliteDataReader::extensions() const
{
    return kExtensions;
}

bool SqliteDataReader::canRead(const QString &filePath) const
{
    return kExtensions.contains(QFileInfo(filePath).suffix().toLower());
}

QStringList SqliteDataReader::listSubSources(const QString &filePath) const
{
    return m_db->tables(filePath);
}

Series SqliteDataReader::read(const QString &source)
{
    Series series;
    const auto [path, requestedTable] = splitSource(source);

    const QStringList tables = m_db->tables(path);
    if (tables.isEmpty())
        return series;

    // Имя таблицы — недоверенный ввод: берём только реально существующую
    // (защита от инъекции через интерполяцию идентификатора).
    QString table;
    if (requestedTable.isEmpty())
        table = tables.first();
    else if (tables.contains(requestedTable))
        table = requestedTable;
    if (table.isEmpty())
        return series;

    series.name = table;
    series.points = m_db->readSeries(path, table, kLabelColumn, kValueColumn);
    return series;
}
