#include "print/PdfChartPrinter.h"

#include <QPainter>
#include <QtPrintSupport/QPrinter>

QT_CHARTS_USE_NAMESPACE

bool PdfChartPrinter::print(QChartView *view, const QString &filePath, bool grayscale)
{
    if (!view)
        return false;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    // Установка ч/б цвета при печати.
    printer.setColorMode(grayscale ? QPrinter::GrayScale : QPrinter::Color);

    QPainter painter(&printer);
    if (!painter.isActive())
        return false;
    painter.setRenderHint(QPainter::Antialiasing);
    view->render(&painter);
    painter.end();
    return true;
}
