#ifndef CHARTMODEL_H
#define CHARTMODEL_H

#include <QObject>
#include <QString>
#include <memory>

#include "data/ChartData.h"
#include "data/IReaderRegistry.h"

/**
 * @brief Модель (M в MVC): загруженные данные + параметры отображения.
 *
 * Хранит ряд и выбранные builder/style, читает данные через реестр читателей и
 * уведомляет наблюдателей сигналами. Не знает о View и о конкретных парсерах.
 */
class ChartModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartModel(std::shared_ptr<IReaderRegistry> registry, QObject *parent = nullptr);

    bool hasData() const;
    const Series &data() const;
    const QString &builder() const;
    const QString &style() const;

    /// @brief Под-источники файла (таблицы БД) для выбора пользователем (без мутации).
    QStringList listSubSources(const QString &filePath) const;

public slots:
    /// @brief Загрузить источник ("путь" или "путь|таблица"); эмитит dataChanged/errorOccurred.
    void setSource(const QString &source);
    /// @brief Сменить тип графика; при изменении эмитит renderOptionsChanged.
    void setBuilder(const QString &builder);
    /// @brief Сменить стиль; при изменении эмитит renderOptionsChanged.
    void setStyle(const QString &style);

signals:
    void dataChanged();          ///< данные перечитаны — перестроить график
    void renderOptionsChanged(); ///< сменился builder/style — перерисовать из тех же данных
    void errorOccurred(const QString &message);

private:
    std::shared_ptr<IReaderRegistry> m_registry;
    Series m_data;
    bool m_hasData = false;
    QString m_builder;
    QString m_style;
};

#endif // CHARTMODEL_H
