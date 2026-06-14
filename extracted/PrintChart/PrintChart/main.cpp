#include "mainwindow.h"
#include <QApplication>
#include <QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPainter>
#include <QPdfWriter>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//MainWindow w;
	//w.show();

	QtCharts::QChartView *chartView = new QtCharts::QChartView();
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->resize(640, 480);

	QtCharts::QChart *chart = chartView->chart();
	chart->setTitle("Beautiful Pie Chart");
	chart->legend()->hide();

	QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
	float hits = 73.0f, misses = 27.0f;
	series->append("Hits", hits);
	series->append("Misses", misses);

	QtCharts::QPieSlice *hit_slice = series->slices().at(0);
	hit_slice->setBrush(QColor(87, 147, 243));  // blue

	QtCharts::QPieSlice *miss_slice = series->slices().at(1);
	miss_slice->setBrush(QColor(221, 68, 68)); // red
	chart->addSeries(series);
	
	// Due to Qt bug, must show() the chart before render()
	// or it will be draw too tiny in the PDF by QPainter

	chartView->show();
	//Печать графика с помощью QPdfWriter(класс для создания pdf файлов) 
	
	QPdfWriter writer("out.pdf");
	
	writer.setCreator("Someone");//Указываем создателя документа
	
	writer.setPageSize(QPagedPaintDevice::A4);//Устанавливаем размер страницы
	
	QPainter painter(&writer);
	
	chartView->render(&painter);
	painter.end();
	
	return a.exec();
}

