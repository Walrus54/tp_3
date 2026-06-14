#ifndef SQLITEDATAREADER_H
#define SQLITEDATAREADER_H

#include <memory>

#include "data/IDataReader.h"
#include "data/IDatabaseManager.h"

/**
 * @brief Читатель данных из БД SQLite.
 *
 * Таблицы хранят колонки [Time, Value]. Поддерживает несколько таблиц в одном
 * файле: source = "путь|таблица". Доступ к БД делегирует IDatabaseManager
 * (внедрение зависимости) — сам класс не зависит от QtSql.
 */
class SqliteDataReader : public IDataReader
{
public:
    explicit SqliteDataReader(std::shared_ptr<IDatabaseManager> db);

    QStringList extensions() const override;
    bool canRead(const QString &filePath) const override;
    QStringList listSubSources(const QString &filePath) const override;
    Series read(const QString &source) override;

private:
    std::shared_ptr<IDatabaseManager> m_db;
};

#endif // SQLITEDATAREADER_H
