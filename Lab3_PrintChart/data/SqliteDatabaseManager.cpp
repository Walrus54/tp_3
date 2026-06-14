#include "data/SqliteDatabaseManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <atomic>

namespace
{
// Уникальные имена соединений: QtSql требует уникальный connectionName, иначе
// повторный addDatabase перетирает предыдущее соединение.
std::atomic<int> g_connCounter{0};

QString nextConnectionName(const QString &tag)
{
    return "sqlite_" + tag + "_" + QString::number(g_connCounter++);
}

// Идентификатор (таблица/колонка) нельзя передать как bind-параметр, поэтому
// экранируем кавычками. Таблицу вызывающий уже проверил по списку tables().
QString quoteIdentifier(const QString &id)
{
    QString escaped = id;
    escaped.replace('"', "\"\"");
    return '"' + escaped + '"';
}
} // namespace

QStringList SqliteDatabaseManager::tables(const QString &dbPath) const
{
    QStringList result;
    const QString connName = nextConnectionName("list");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbPath);
        if (db.open()) {
            result = db.tables(QSql::Tables);
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(connName);
    return result;
}

QVector<ChartPoint> SqliteDatabaseManager::readSeries(const QString &dbPath,
                                                      const QString &table,
                                                      const QString &labelColumn,
                                                      const QString &valueColumn) const
{
    QVector<ChartPoint> points;
    const QString connName = nextConnectionName("read");
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbPath);
        if (db.open()) {
            const QString sql = QStringLiteral("SELECT %1, %2 FROM %3")
                                    .arg(quoteIdentifier(labelColumn),
                                         quoteIdentifier(valueColumn),
                                         quoteIdentifier(table));
            QSqlQuery query(db);
            if (query.exec(sql))
                while (query.next())
                    points.push_back({query.value(0).toString(), query.value(1).toDouble()});
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(connName);
    return points;
}
