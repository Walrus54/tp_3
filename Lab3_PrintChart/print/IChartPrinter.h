#ifndef ICHARTPRINTER_H
#define ICHARTPRINTER_H

#include <QString>
#include <QtCharts/QChartView>

/**
 * @brief Абстракция печати графика. UI зависит только от интерфейса.
 */
class IChartPrinter
{
public:
    virtual ~IChartPrinter() = default;

    /**
     * @brief Напечатать содержимое представления графика в файл.
     * @param view представление графика.
     * @param filePath путь выходного файла.
     * @param grayscale печать в оттенках серого.
     * @return true при успехе.
     */
    virtual bool print(QtCharts::QChartView *view,
                       const QString &filePath,
                       bool grayscale) = 0;
};

#endif // ICHARTPRINTER_H
