#ifndef SQLITEDATABASEMANAGER_H
#define SQLITEDATABASEMANAGER_H

#include "data/IDatabaseManager.h"

/**
 * @brief Доступ к SQLite через QtSql.
 *
 * Управляет временным именованным соединением на каждый запрос и закрывает его.
 */
class SqliteDatabaseManager : public IDatabaseManager
{
public:
    QStringList tables(const QString &dbPath) const override;

    QVector<ChartPoint> readSeries(const QString &dbPath,
                                   const QString &table,
                                   const QString &labelColumn,
                                   const QString &valueColumn) const override;
};

#endif // SQLITEDATABASEMANAGER_H
