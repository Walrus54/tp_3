#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QMap>
#include <QObject>
#include <QString>
#include <functional>
#include <memory>

#include "charts/IChartBuilder.h"
#include "charts/IChartStyle.h"
#include "print/IChartPrinter.h"
#include "ui/ChartModel.h"

QT_BEGIN_NAMESPACE
namespace QtCharts { class QChart; class QChartView; }
QT_END_NAMESPACE

/// Именованные фабрики «плагинов» (выбор по имени из выпадающего списка).
using BuilderFactory = QMap<QString, std::function<std::shared_ptr<IChartBuilder>()>>;
using StyleFactory = QMap<QString, std::function<std::shared_ptr<IChartStyle>()>>;

/**
 * @brief Контроллер (C в MVC): посредник между View и Model.
 *
 * Принимает намерения пользователя от View (выбор файла, типа графика, стиля,
 * печать), транслирует их в мутации модели, по сигналам модели собирает QChart
 * и отдаёт его View сигналом chartReady. View не знает ни о парсерах, ни о
 * билдерах/стилях, ни о принтере.
 */
class ChartController : public QObject
{
    Q_OBJECT
public:
    ChartController(BuilderFactory builders,
                    StyleFactory styles,
                    std::shared_ptr<IChartPrinter> printer,
                    std::unique_ptr<ChartModel> model,
                    QObject *parent = nullptr);
    ~ChartController() override;

    /// @brief Доступные типы графиков (для заполнения списка во View).
    QStringList chartTypes() const;

    /// @brief Под-источники файла (таблицы БД) для выбора пользователем.
    QStringList subSourcesFor(const QString &filePath) const;

    bool hasData() const;

public slots:
    /// @brief Выбрать источник данных ("путь" или "путь|таблица").
    void selectSource(const QString &source);
    /// @brief Выбрать тип графика по имени.
    void selectChartType(const QString &name);
    /// @brief Переключить стиль: чёрно-белый (true) или цветной (false).
    void selectStyle(bool grayscale);
    /**
     * @brief Напечатать текущий график в файл.
     * @param view представление графика, содержимое которого рендерится.
     * @param filePath путь выходного PDF.
     * @return true при успехе.
     */
    bool printChart(QtCharts::QChartView *view, const QString &filePath) const;

signals:
    /// @brief Готов новый график; владение QChart передаётся получателю (View).
    void chartReady(QtCharts::QChart *chart);
    void errorOccurred(const QString &message);

private slots:
    void rebuildChart();

private:
    QtCharts::QChart *buildChart() const;

    BuilderFactory m_builders;
    StyleFactory m_styles;
    std::shared_ptr<IChartPrinter> m_printer;
    std::unique_ptr<ChartModel> m_model;
    bool m_grayscale = false;
};

#endif // CHARTCONTROLLER_H
