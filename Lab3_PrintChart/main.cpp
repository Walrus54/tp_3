#include <QApplication>
#include <functional>
#include <memory>

#include "charts/BarChartBuilder.h"
#include "charts/ColorChartStyle.h"
#include "charts/GrayChartStyle.h"
#include "charts/PieChartBuilder.h"
#include "controller/ChartController.h"
#include "data/IDatabaseManager.h"
#include "data/JsonDataReader.h"
#include "data/ReaderRegistry.h"
#include "data/SqliteDataReader.h"
#include "data/SqliteDatabaseManager.h"
#include "ioc/IocContainer.h"
#include "print/PdfChartPrinter.h"
#include "ui/ChartModel.h"
#include "ui/MainWindow.h"

// Composition Root — единственное место, где упоминаются конкретные классы.
// Здесь собирается граф зависимостей через IoC-контейнер, а набор «плагинов»
// (графики, стили) — через именованные фабрики.
static MainWindow *createMainWindow()
{
    IocContainer ioc;

    // Доступ к БД прячем за интерфейс; SqliteDataReader получает его как
    // зависимость конструктора (внедрение зависимости).
    ioc.RegisterFactory<IDatabaseManager, SqliteDatabaseManager>();
    ioc.RegisterFactory<SqliteDataReader, SqliteDataReader, IDatabaseManager>();
    ioc.RegisterFactory<JsonDataReader, JsonDataReader>();

    // Реестр читателей собираем функтором — контейнер сам внедряет читатели.
    ioc.RegisterFunctor<IReaderRegistry, SqliteDataReader, JsonDataReader>(
        std::function<std::shared_ptr<IReaderRegistry>(
            std::shared_ptr<SqliteDataReader>, std::shared_ptr<JsonDataReader>)>(
            [](std::shared_ptr<SqliteDataReader> sqlite,
               std::shared_ptr<JsonDataReader> json) {
                auto registry = std::make_shared<ReaderRegistry>();
                registry->add(sqlite);
                registry->add(json);
                return registry;
            }));

    // Принтер — обычная фабрика без зависимостей.
    ioc.RegisterFactory<IChartPrinter, PdfChartPrinter>();

    auto registry = ioc.GetObject<IReaderRegistry>();
    auto printer = ioc.GetObject<IChartPrinter>();

    // Виды графиков: новый график -> одна строка.
    BuilderFactory builders;
    builders["BarChart"] = [] { return std::make_shared<BarChartBuilder>(); };
    builders["PieChart"] = [] { return std::make_shared<PieChartBuilder>(); };

    // Стили (цветной / черно-белый).
    StyleFactory styles;
    styles["color"] = [] { return std::make_shared<ColorChartStyle>(); };
    styles["gray"] = [] { return std::make_shared<GrayChartStyle>(); };

    auto model = std::make_unique<ChartModel>(registry);
    auto controller = std::make_unique<ChartController>(
        std::move(builders), std::move(styles), printer, std::move(model));

    return new MainWindow(std::move(controller));
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow *window = createMainWindow();
    window->show();

    return app.exec();
}
