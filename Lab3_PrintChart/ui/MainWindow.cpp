#include "ui/MainWindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <utility>

#include "controller/ChartController.h"

QT_CHARTS_USE_NAMESPACE

namespace
{
constexpr int kWindowWidth = 1200;
constexpr int kWindowHeight = 600;
constexpr int kFileNameColumnWidth = 200;
// Соотношение ширин панелей в сплиттере: список файлов vs график.
constexpr int kFileListStretch = 1;
constexpr int kChartStretch = 3;
// Разделитель source: "путь|под-источник".
const QString kSourceSeparator = "|";
} // namespace

MainWindow::MainWindow(std::unique_ptr<ChartController> controller, QWidget *parent)
    : QMainWindow(parent)
    , m_controller(std::move(controller))
{
    buildUi();

    // View реагирует только на сигналы контроллера.
    connect(m_controller.get(), &ChartController::chartReady, this, &MainWindow::displayChart);
    connect(m_controller.get(), &ChartController::errorOccurred, this, &MainWindow::showError);
}

MainWindow::~MainWindow() = default;

void MainWindow::buildUi()
{
    setWindowTitle("PrintChart");
    resize(kWindowWidth, kWindowHeight);

    // --- Левая часть: список файлов (выбор файла с данными) ---
    m_fsModel = new QFileSystemModel(this);
    m_fsModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    const QString startPath = QDir::homePath();
    m_fsModel->setRootPath(startPath);

    m_fileView = new QTableView(this);
    m_fileView->setModel(m_fsModel);
    m_fileView->setRootIndex(m_fsModel->index(startPath));
    m_fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_fileView->horizontalHeader()->resizeSection(0, kFileNameColumnWidth);
    connect(m_fileView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onFileSelectionChanged);

    // --- Верхняя панель управления ---
    m_chartTypeBox = new QComboBox(this);
    m_chartTypeBox->addItems(m_controller->chartTypes());
    connect(m_chartTypeBox, &QComboBox::currentTextChanged,
            m_controller.get(), &ChartController::selectChartType);

    m_grayCheck = new QCheckBox("Черно-белый график", this);
    connect(m_grayCheck, &QCheckBox::toggled, m_controller.get(), &ChartController::selectStyle);

    m_openDirButton = new QPushButton("Папка с данными…", this);
    connect(m_openDirButton, &QPushButton::clicked, this, &MainWindow::onOpenDirClicked);

    m_printButton = new QPushButton("Печать графика", this);
    connect(m_printButton, &QPushButton::clicked, this, &MainWindow::onPrintClicked);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->addWidget(m_openDirButton);
    topBar->addStretch();
    topBar->addWidget(new QLabel("Выберите тип диаграммы", this));
    topBar->addWidget(m_chartTypeBox);
    topBar->addWidget(m_grayCheck);
    topBar->addWidget(m_printButton);

    // --- Правая часть: график ---
    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setChart(new QChart());

    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(m_fileView);
    splitter->addWidget(m_chartView);
    splitter->setStretchFactor(0, kFileListStretch);
    splitter->setStretchFactor(1, kChartStretch);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topBar);
    mainLayout->addWidget(splitter);

    QWidget *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    statusBar()->showMessage("Выберите файл с данными");
}

void MainWindow::onFileSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    const QModelIndexList indexes = selected.indexes();
    if (indexes.isEmpty())
        return;

    const QString filePath = m_fsModel->filePath(indexes.first());
    statusBar()->showMessage("Выбранный путь : " + filePath);

    // Если в файле несколько под-источников (таблиц БД) — даём выбрать.
    QString source = filePath;
    const QStringList subs = m_controller->subSourcesFor(filePath);
    if (subs.size() > 1) {
        bool ok = false;
        const QString table = QInputDialog::getItem(
            this, "Выбор таблицы", "Таблица:", subs, 0, false, &ok);
        if (!ok)
            return;
        source = filePath + kSourceSeparator + table;
    }

    m_controller->selectSource(source);
}

void MainWindow::onOpenDirClicked()
{
    // Выбор папки, файлы которой показываем в левой панели.
    const QString current = m_fsModel->filePath(m_fileView->rootIndex());
    const QString dir = QFileDialog::getExistingDirectory(
        this, "Выберите папку с данными", current.isEmpty() ? QDir::homePath() : current);
    if (dir.isEmpty())
        return;
    m_fsModel->setRootPath(dir);
    m_fileView->setRootIndex(m_fsModel->index(dir));
    statusBar()->showMessage("Папка : " + dir);
}

void MainWindow::onPrintClicked()
{
    if (!m_controller->hasData()) {
        QMessageBox::information(this, "Печать", "Сначала выберите файл с данными.");
        return;
    }

    // При печати в pdf выбираем место сохранения графика.
    const QString filePath = QFileDialog::getSaveFileName(
        this, "Сохранить график в PDF", QDir::homePath() + "/chart.pdf",
        "PDF файлы (*.pdf)");
    if (filePath.isEmpty())
        return;

    if (m_controller->printChart(m_chartView, filePath))
        statusBar()->showMessage("График сохранён : " + filePath);
    else
        QMessageBox::warning(this, "Печать", "Не удалось сохранить PDF.");
}

void MainWindow::displayChart(QChart *chart)
{
    QChart *old = m_chartView->chart();
    m_chartView->setChart(chart);
    delete old;
    statusBar()->showMessage("График построен");
}

void MainWindow::showError(const QString &message)
{
    statusBar()->showMessage(message);
}
