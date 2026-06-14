#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <functional>
#include <memory>

#include "charts/IChartBuilder.h"
#include "charts/IChartStyle.h"
#include "data/ChartData.h"
#include "data/IReaderRegistry.h"

QT_BEGIN_NAMESPACE
namespace QtCharts { class QChart; }
QT_END_NAMESPACE

/// Именованные фабрики «плагинов» (выбор по имени из выпадающего списка).
using BuilderFactory = QMap<QString, std::function<std::shared_ptr<IChartBuilder>()>>;
using StyleFactory = QMap<QString, std::function<std::shared_ptr<IChartStyle>()>>;

/**
 * @brief Модель (M в MVC): данные + параметры отображения + сборка графика.
 *
 * Хранит ряд и выбранные builder/style, читает данные через реестр читателей,
 * собирает QChart выбранным билдером в выбранном стиле и отдаёт его сигналом
 * chartReady. View связывается с моделью напрямую сигнал-слотовыми соединениями
 * — отдельного контроллера в схеме нет. Модель не знает о View, о конкретных
 * парсерах и о принтере.
 */
class ChartModel : public QObject
{
    Q_OBJECT
public:
    ChartModel(std::shared_ptr<IReaderRegistry> registry,
               BuilderFactory builders,
               StyleFactory styles,
               QObject *parent = nullptr);

    bool hasData() const;

    /// @brief Доступные типы графиков (для заполнения списка во View).
    QStringList chartTypes() const;

    /// @brief Под-источники файла (таблицы БД) для выбора пользователем (без мутации).
    QStringList listSubSources(const QString &filePath) const;

public slots:
    /// @brief Загрузить источник ("путь" или "путь|таблица"); пересобирает график.
    void setSource(const QString &source);
    /// @brief Сменить тип графика по имени; пересобирает график.
    void setChartType(const QString &name);
    /// @brief Переключить стиль: чёрно-белый (true) или цветной (false); пересобирает график.
    void setGrayscale(bool grayscale);

signals:
    /// @brief Готов новый график; владение QChart передаётся получателю (View).
    void chartReady(QtCharts::QChart *chart);
    void errorOccurred(const QString &message);

private:
    void rebuildChart();
    QtCharts::QChart *buildChart() const;

    std::shared_ptr<IReaderRegistry> m_registry;
    BuilderFactory m_builders;
    StyleFactory m_styles;
    Series m_data;
    bool m_hasData = false;
    QString m_builder;
    QString m_style;
};

#endif // CHARTMODEL_H
