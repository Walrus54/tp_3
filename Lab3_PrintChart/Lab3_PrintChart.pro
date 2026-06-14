QT       += core gui widgets charts sql printsupport

CONFIG += c++17

# Снижаем связность за счёт интерфейсов, IoC-контейнера и MVC.

INCLUDEPATH += $$PWD

SOURCES += \
    main.cpp \
    data/ReaderRegistry.cpp \
    data/SqliteDatabaseManager.cpp \
    data/SqliteDataReader.cpp \
    data/JsonDataReader.cpp \
    data/Aggregator.cpp \
    charts/BarChartBuilder.cpp \
    charts/PieChartBuilder.cpp \
    charts/ColorChartStyle.cpp \
    charts/GrayChartStyle.cpp \
    print/PdfChartPrinter.cpp \
    controller/ChartController.cpp \
    ui/ChartModel.cpp \
    ui/MainWindow.cpp

HEADERS += \
    ioc/IocContainer.h \
    data/ChartData.h \
    data/Aggregator.h \
    data/IDataReader.h \
    data/IReaderRegistry.h \
    data/ReaderRegistry.h \
    data/IDatabaseManager.h \
    data/SqliteDatabaseManager.h \
    data/SqliteDataReader.h \
    data/JsonDataReader.h \
    charts/IPalette.h \
    charts/IChartBuilder.h \
    charts/IChartStyle.h \
    charts/BarChartBuilder.h \
    charts/PieChartBuilder.h \
    charts/ColorChartStyle.h \
    charts/GrayChartStyle.h \
    print/IChartPrinter.h \
    print/PdfChartPrinter.h \
    controller/ChartController.h \
    ui/ChartModel.h \
    ui/MainWindow.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
