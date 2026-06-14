#ifndef PDFCHARTPRINTER_H
#define PDFCHARTPRINTER_H

#include "print/IChartPrinter.h"

/**
 * @brief Печать графика в PDF-файл через QPrinter.
 */
class PdfChartPrinter : public IChartPrinter
{
public:
    bool print(QtCharts::QChartView *view,
               const QString &filePath,
               bool grayscale) override;
};

#endif // PDFCHARTPRINTER_H
