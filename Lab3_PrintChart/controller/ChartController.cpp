#include "controller/ChartController.h"

#include <QtCharts/QChart>

#include <utility>

QT_CHARTS_USE_NAMESPACE

namespace
{
// Имена стилей, под которыми они зарегистрированы в композиционном корне.
const QString kStyleColor = "color";
const QString kStyleGray = "gray";
} // namespace

ChartController::ChartController(BuilderFactory builders,
                                StyleFactory styles,
                                std::shared_ptr<IChartPrinter> printer,
                                std::unique_ptr<ChartModel> model,
                                QObject *parent)
    : QObject(parent)
    , m_builders(std::move(builders))
    , m_styles(std::move(styles))
    , m_printer(std::move(printer))
    , m_model(std::move(model))
{
    connect(m_model.get(), &ChartModel::dataChanged, this, &ChartController::rebuildChart);
    connect(m_model.get(), &ChartModel::renderOptionsChanged, this, &ChartController::rebuildChart);
    connect(m_model.get(), &ChartModel::errorOccurred, this, &ChartController::errorOccurred);

    // Параметры отображения по умолчанию.
    if (!m_builders.isEmpty())
        m_model->setBuilder(m_builders.firstKey());
    m_model->setStyle(kStyleColor);
}

ChartController::~ChartController() = default;

QStringList ChartController::chartTypes() const
{
    return m_builders.keys();
}

QStringList ChartController::subSourcesFor(const QString &filePath) const
{
    return m_model->listSubSources(filePath);
}

bool ChartController::hasData() const
{
    return m_model->hasData();
}

void ChartController::selectSource(const QString &source)
{
    m_model->setSource(source);
}

void ChartController::selectChartType(const QString &name)
{
    m_model->setBuilder(name);
}

void ChartController::selectStyle(bool grayscale)
{
    m_grayscale = grayscale;
    m_model->setStyle(grayscale ? kStyleGray : kStyleColor);
}

bool ChartController::printChart(QChartView *view, const QString &filePath) const
{
    return m_printer->print(view, filePath, m_grayscale);
}

void ChartController::rebuildChart()
{
    if (!m_model->hasData())
        return;
    if (QChart *chart = buildChart())
        emit chartReady(chart);
}

QChart *ChartController::buildChart() const
{
    auto builderFactory = m_builders.value(m_model->builder());
    auto styleFactory = m_styles.value(m_model->style());
    if (!builderFactory || !styleFactory)
        return nullptr;

    auto builder = builderFactory();
    auto style = styleFactory();

    // Билдер красит элементы палитрой стиля, затем стиль задаёт общую тему/фон.
    QChart *chart = builder->build(m_model->data(), *style);
    style->apply(chart);
    return chart;
}
