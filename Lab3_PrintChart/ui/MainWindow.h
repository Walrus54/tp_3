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

class ChartController;

/**
 * @brief Главное окно — View в MVC.
 *
 * Только виджеты и проводка сигналов: пользовательские намерения уходят в
 * ChartController, готовый график приходит обратно сигналом и отображается.
 * View не знает о парсерах, билдерах, стилях и принтере. Вся вёрстка — на C++
 * (без .ui-форм).
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(std::unique_ptr<ChartController> controller, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onFileSelectionChanged(const QItemSelection &selected, const QItemSelection &);
    void onOpenDirClicked();
    void onPrintClicked();
    void displayChart(QtCharts::QChart *chart);
    void showError(const QString &message);

private:
    void buildUi();

    std::unique_ptr<ChartController> m_controller;

    QFileSystemModel *m_fsModel;
    QTableView *m_fileView;
    QComboBox *m_chartTypeBox;
    QCheckBox *m_grayCheck;
    QPushButton *m_openDirButton;
    QPushButton *m_printButton;
    QtCharts::QChartView *m_chartView;
};

#endif // MAINWINDOW_H
