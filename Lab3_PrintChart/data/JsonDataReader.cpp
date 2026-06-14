#include "data/JsonDataReader.h"

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace
{
const QString kExtension = "json";

// Ключи фиксированного формата [дата, значение].
const QString kDateKey = "date";
const QString kValueKey = "value";
const QString kNameKey = "name";
const QString kPointsKey = "points";

ChartPoint pointFromObject(const QJsonObject &obj)
{
    return {obj.value(kDateKey).toVariant().toString(),
            obj.value(kValueKey).toVariant().toDouble()};
}

void appendPoints(const QJsonArray &array, Series &series)
{
    for (const QJsonValue &v : array)
        if (v.isObject())
            series.points.push_back(pointFromObject(v.toObject()));
}
} // namespace

QStringList JsonDataReader::extensions() const
{
    return {kExtension};
}

bool JsonDataReader::canRead(const QString &filePath) const
{
    return QFileInfo(filePath).suffix().toLower() == kExtension;
}

QStringList JsonDataReader::listSubSources(const QString &filePath) const
{
    Q_UNUSED(filePath);
    return {}; // JSON — одиночный ряд, под-источников нет.
}

Series JsonDataReader::read(const QString &source)
{
    Series series;

    QFile file(source);
    if (!file.open(QIODevice::ReadOnly))
        return series;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (doc.isArray()) {
        series.name = QFileInfo(source).completeBaseName();
        appendPoints(doc.array(), series);
    } else if (doc.isObject()) {
        const QJsonObject root = doc.object();
        series.name = root.value(kNameKey).toString();
        if (series.name.isEmpty())
            series.name = QFileInfo(source).completeBaseName();
        appendPoints(root.value(kPointsKey).toArray(), series);
    }
    return series;
}
