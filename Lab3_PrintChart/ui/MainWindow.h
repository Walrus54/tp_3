#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QPushButton;
class QTableView;
class QFileSystemModel;
class QItemSelection;
namespace QtCharts { class QChart; class QChartView; }
QT_END_NAMESPACE

class ChartModel;
class IChartPrinter;

/**
 * @brief Главное окно — View в MVC.
 *
 * Только виджеты и проводка сигналов: намерения пользователя сигнал-слотовыми
 * соединениями уходят прямо в модель, готовый график приходит обратно сигналом
 * chartReady и отображается. Отдельного контроллера в схеме нет — его роль
 * выполняют эти соединения. Печать выполняется сервисом печати. View не знает о
 * парсерах, билдерах и стилях. Вся вёрстка — на C++ (без .ui-форм).
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(std::unique_ptr<ChartModel> model,
               std::shared_ptr<IChartPrinter> printer,
               QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onFileSelectionChanged(const QItemSelection &selected, const QItemSelection &);
    void onOpenDirClicked();
    void onPrintClicked();
    void displayChart(QtCharts::QChart *chart);
    void showError(const QString &message);

private:
    void buildUi();

    std::unique_ptr<ChartModel> m_model;
    std::shared_ptr<IChartPrinter> m_printer;

    QFileSystemModel *m_fsModel;
    QTableView *m_fileView;
    QComboBox *m_chartTypeBox;
    QCheckBox *m_grayCheck;
    QPushButton *m_openDirButton;
    QPushButton *m_printButton;
    QtCharts::QChartView *m_chartView;
};

#endif // MAINWINDOW_H
