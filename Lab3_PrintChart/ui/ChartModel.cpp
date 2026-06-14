#include "ui/ChartModel.h"

#include <QtCharts/QChart>

#include <utility>

QT_CHARTS_USE_NAMESPACE

namespace
{
// Имена стилей, под которыми они зарегистрированы в композиционном корне.
const QString kStyleColor = "color";
const QString kStyleGray = "gray";

// Путь к файлу из source ("путь" либо "путь|таблица").
QString sourcePath(const QString &source)
{
    const int pos = source.indexOf('|');
    return pos < 0 ? source : source.left(pos);
}
} // namespace

ChartModel::ChartModel(std::shared_ptr<IReaderRegistry> registry,
                       BuilderFactory builders,
                       StyleFactory styles,
                       QObject *parent)
    : QObject(parent)
    , m_registry(std::move(registry))
    , m_builders(std::move(builders))
    , m_styles(std::move(styles))
{
    // Параметры отображения по умолчанию.
    if (!m_builders.isEmpty())
        m_builder = m_builders.firstKey();
    m_style = kStyleColor;
}

bool ChartModel::hasData() const
{
    return m_hasData;
}

QStringList ChartModel::chartTypes() const
{
    return m_builders.keys();
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
    rebuildChart();
}

void ChartModel::setChartType(const QString &name)
{
    if (name == m_builder)
        return;
    m_builder = name;
    rebuildChart();
}

void ChartModel::setGrayscale(bool grayscale)
{
    const QString style = grayscale ? kStyleGray : kStyleColor;
    if (style == m_style)
        return;
    m_style = style;
    rebuildChart();
}

void ChartModel::rebuildChart()
{
    if (!m_hasData)
        return;
    if (QChart *chart = buildChart())
        emit chartReady(chart);
}

QChart *ChartModel::buildChart() const
{
    auto builderFactory = m_builders.value(m_builder);
    auto styleFactory = m_styles.value(m_style);
    if (!builderFactory || !styleFactory)
        return nullptr;

    auto builder = builderFactory();
    auto style = styleFactory();

    // Билдер красит элементы палитрой стиля, затем стиль задаёт общую тему/фон.
    QChart *chart = builder->build(m_data, *style);
    style->apply(chart);
    return chart;
}
