#ifndef IPALETTE_H
#define IPALETTE_H

class QColor;

/**
 * @brief Поставщик цветов по индексу элемента.
 *
 * Билдеры запрашивают цвет по индексу серии/сектора, не зная о конкретном
 * стиле. Раскраска живёт там, где известна структура series, а стили не делают
 * qobject_cast по типам Qt.
 */
class IPalette
{
public:
    virtual ~IPalette() = default;

    /**
     * @brief Цвет для элемента.
     * @param index индекс элемента.
     * @param total всего элементов (нужно стилям с градиентом).
     */
    virtual QColor colorFor(int index, int total) const = 0;
};

#endif // IPALETTE_H
