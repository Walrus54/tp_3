#include "ui/ChartModel.h"

#include <utility>

namespace
{
// Путь к файлу из source ("путь" либо "путь|таблица").
QString sourcePath(const QString &source)
{
    const int pos = source.indexOf('|');
    return pos < 0 ? source : source.left(pos);
}
} // namespace

ChartModel::ChartModel(std::shared_ptr<IReaderRegistry> registry, QObject *parent)
    : QObject(parent)
    , m_registry(std::move(registry))
{
}

bool ChartModel::hasData() const
{
    return m_hasData;
}

const Series &ChartModel::data() const
{
    return m_data;
}

const QString &ChartModel::builder() const
{
    return m_builder;
}

const QString &ChartModel::style() const
{
    return m_style;
}

QStringList ChartModel::listSubSources(const QString &filePath) const
{
    auto reader = m_registry->readerFor(filePath);
    return reader ? reader->listSubSources(filePath) : QStringList{};
}

void ChartModel::setSource(const QString &source)
{
    auto reader = m_registry->readerFor(sourcePath(source));
    if (!reader) {
        emit errorOccurred("Неподдерживаемый формат: " + source);
        return;
    }

    Series series = reader->read(source);
    if (series.points.isEmpty()) {
        emit errorOccurred("Не удалось прочитать данные: " + source);
        return;
    }

    m_data = std::move(series);
    m_hasData = true;
    emit dataChanged();
}

void ChartModel::setBuilder(const QString &builder)
{
    if (builder == m_builder)
        return;
    m_builder = builder;
    emit renderOptionsChanged();
}

void ChartModel::setStyle(const QString &style)
{
    if (style == m_style)
        return;
    m_style = style;
    emit renderOptionsChanged();
}
