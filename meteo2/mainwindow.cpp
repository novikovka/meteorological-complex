#include "mainwindow.h"
#include "resultswindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <unordered_map>

#include "fileparser.h"
#include "analyzer.h"
#include "types.h"
#include "displaymanager.h"

#include <vector>
#include <array>
#include <map>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //устанавливаем размер окна "на весь экран"
    showMaximized();

    // запрещаем редактирование ячеек
    ui->TableMtd->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //включаем отслеживание движения мыши
    ui->TableMtd->setMouseTracking(true);

    // переход по клику на ячейку TableMtd
    connect(ui->TableMtd, &QTableWidget::cellClicked,
            this, &MainWindow::onTableMtdClicked);

    // переход по клику на ячейку TableMts
    connect(ui->TableMts, &QTableWidget::cellClicked,
            this, &MainWindow::onTableMtsClicked);

    // для изменения вида курсора
    connect(ui->TableMtd, &QTableWidget::cellEntered,
            this, &MainWindow::onTableCellEntered);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTableCellEntered(int row, int column)
{
    QTableWidget* table = qobject_cast<QTableWidget*>(sender());
    if (!table) return;

    if (column == 0)
        table->unsetCursor();
    else
        table->setCursor(Qt::PointingHandCursor);
}

void MainWindow::onTableMtdClicked(int row, int column)
{
    QTableWidget* table = qobject_cast<QTableWidget*>(sender()); //ссылка на таблицу

    if (!table)
        return;

    QTableWidgetItem* cellItem = table->item(row, column);
    if (!cellItem)
        return;

    // создаём структуру данных
    CellInfo cell;

    cell.row = row;
    cell.column = column;
    cell.tableName = table->objectName();
    cell.cellValue = cellItem->text();

    QTableWidgetItem* hItem = table->item(row, 0); //получаем высоту
    cell.height = hItem->text().toDouble();

    QTableWidgetItem* headerItem = table->horizontalHeaderItem(column);

    if (headerItem)
        cell.columnName = headerItem->text();
    else
        cell.columnName = QString("Column %1").arg(column);

    QString html;

    /*
    Zone currentZone{};
    for (const Zone& item : zones){
        if (std::abs(item.height - cell.height) < 100.0){
            currentZone = item;
            break;
        }
    }
    */
    Zone currentZone{};

    // Предполагаем, что zones отсортированы по height
    if (!zones.empty()) {
        // Находим первую зону с высотой >= cell.height
        auto it = std::lower_bound(zones.begin(), zones.end(), cell.height,
            [](const Zone& zone, double height) {
                return zone.height < height;
            });

        if (it == zones.begin()) {
            // Все зоны выше cell.height, берем первую
            currentZone = zones.front();
        }
        else if (it == zones.end()) {
            // Все зоны ниже cell.height, берем последнюю
            currentZone = zones.back();
        }
        else {
            // Сравниваем две соседние зоны: (it-1) и it
            const Zone& lower = *(it - 1);
            const Zone& upper = *it;

            double diffLower = std::abs(lower.height - cell.height);
            double diffUpper = std::abs(upper.height - cell.height);

            currentZone = (diffLower <= diffUpper) ? lower : upper;
        }
    }

    if(cell.columnName == "v"){
        html = displayManager.HtmlMtdV(cell, currentZone, mtd, coordinates);
    }else if(cell.columnName == "av"){
        html = displayManager.HtmlMtdAV(cell, currentZone, mtd, coordinates);
    }else if(cell.columnName == "TTi"){
        html = displayManager.HtmlTTiMtd(cell, zones, mtd, globalParam);
    }else if(cell.columnName == "TTcpm"){
        html = displayManager.HtmlTTcpmMtd(cell, zones, mtd, globalParam);
    }else if(cell.columnName == "PPi"){
        html = displayManager.HtmlPPiMtd(cell, zones, mtd);
    }else if(cell.columnName == "PPcpm"){
        html = displayManager.HtmlPPcpmMtd(cell, zones, mtd);
    }

    // выводим
    ui->textBrowser->setHtml(html);

    qDebug() << "Клик по таблице:" << cell.tableName
             << "row:" << row
             << "column:" << column;

}

void MainWindow::onTableMtsClicked(int row, int column)
{
    QTableWidget* table = qobject_cast<QTableWidget*>(sender()); //ссылка на таблицу

    if (!table)
        return;

    QTableWidgetItem* cellItem = table->item(row, column);
    if (!cellItem)
        return;

    // создаём структуру данных
    CellInfo cell;

    cell.row = row;
    cell.column = column;
    cell.tableName = table->objectName();
    cell.cellValue = cellItem->text();

    QTableWidgetItem* hItem = table->item(row, 0); //получаем высоту

    cell.height = hItem->text().toDouble();

    QTableWidgetItem* headerItem = table->horizontalHeaderItem(column);

    if (headerItem)
        cell.columnName = headerItem->text();
    else
        cell.columnName = QString("Column %1").arg(column);

    QString html;

    if(cell.columnName == "w"){
        html = displayManager.HtmlMtsV(cell, zones, mts, coordinates);
    }else if(cell.columnName == "aw"){
        html = displayManager.HtmlMtsAV(cell, zones, mts, coordinates);
    }else if(cell.columnName == "TTi"){
        html = displayManager.HtmlTTiMts(cell, zones, mts, globalParam);
    }else if(cell.columnName == "TTcpm"){
        html = displayManager.HtmlTTcpmMts(cell, zones, mts, globalParam);
    }else if(cell.columnName == "PPi"){
        html = displayManager.HtmlPPiMts(cell, zones, mts);
    }else if(cell.columnName == "PPcpm"){
        html = displayManager.HtmlPPcpmMts(cell, zones, mts);
    }

    // выводим
    ui->textBrowser->setHtml(html);

    qDebug() << "Клик по таблице:" << cell.tableName
             << "row:" << row
             << "column:" << column;

}

void MainWindow::setDataTableMtd(const std::vector<Mtd>& data)
{
    QSignalBlocker blocker(ui->TableMtd_1);

    ui->TableMtd_1->clear();
    ui->TableMtd_1->setRowCount(data.size());
    ui->TableMtd_1->setColumnCount(5);

    QStringList headers;
    headers << "h" << "v" << "av" << "TTi" << "PPi";
    ui->TableMtd_1->setHorizontalHeaderLabels(headers);

    ui->TableMtd_1->setCurrentCell(-1, -1);
    ui->TableMtd_1->clearSelection();

    for (int row = 0; row < data.size(); ++row)
    {
        const Mtd &item = data[row];

        QTableWidgetItem *hItem = new QTableWidgetItem(QString::number(item.h, 'f', 2));
        hItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hItem->setFlags(hItem->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEditable);
        ui->TableMtd_1->setItem(row, 0, hItem);

        QTableWidgetItem *vItem = new QTableWidgetItem(QString::number(item.v, 'f', 2));
        vItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd_1->setItem(row, 1, vItem);

        QTableWidgetItem *avItem = new QTableWidgetItem(QString::number(item.av, 'f', 2));
        avItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd_1->setItem(row, 2, avItem);

        QTableWidgetItem *ttiItem = new QTableWidgetItem(QString::number(item.TTi, 'f', 2));
        ttiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd_1->setItem(row, 3, ttiItem);

        QTableWidgetItem *ppiItem = new QTableWidgetItem(QString::number(item.PPi, 'f', 2));
        ppiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd_1->setItem(row, 4, ppiItem);
    }

    // подгоняем колонки под содержимое
    ui->TableMtd_1->resizeColumnsToContents();

    // отключаем горизонтальный скролл
    ui->TableMtd_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // вычисляем нужную ширину таблицы
    int width = ui->TableMtd_1->verticalHeader()->width();
    for (int i = 0; i < ui->TableMtd_1->columnCount(); ++i)
        width += ui->TableMtd_1->columnWidth(i);

    // добавляем ширину рамки
    width += ui->TableMtd_1->frameWidth() * 2;

    ui->TableMtd_1->setFixedWidth(width);

    ui->TableMtd_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setDataMtd(const std::vector<Mtd>& data)
{
    QSignalBlocker blocker(ui->TableMtd);

    ui->TableMtd->clear();
    ui->TableMtd->setRowCount(data.size());
    ui->TableMtd->setColumnCount(7);

    QStringList headers;
    headers << "h" << "v" << "av" << "TTi" << "TTcpm" << "PPi" << "PPcpm";
    ui->TableMtd->setHorizontalHeaderLabels(headers);

    ui->TableMtd->setCurrentCell(-1, -1);
    ui->TableMtd->clearSelection();

    for (int row = 0; row < data.size(); ++row)
    {
        const Mtd &item = data[row];

        QTableWidgetItem *hItem = new QTableWidgetItem(QString::number(item.h, 'f', 2));
        hItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hItem->setFlags(hItem->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEditable);
        ui->TableMtd->setItem(row, 0, hItem);

        QTableWidgetItem *vItem = new QTableWidgetItem(QString::number(item.v, 'f', 2));
        vItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 1, vItem);

        QTableWidgetItem *avItem = new QTableWidgetItem(QString::number(item.av, 'f', 2));
        avItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 2, avItem);

        QTableWidgetItem *ttiItem = new QTableWidgetItem(QString::number(item.TTi, 'f', 2));
        ttiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 3, ttiItem);

        QTableWidgetItem *ttcpmItem = new QTableWidgetItem(QString::number(item.TTcpm, 'f', 2));
        ttcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 4, ttcpmItem);

        QTableWidgetItem *ppiItem = new QTableWidgetItem(QString::number(item.PPi, 'f', 2));
        ppiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 5, ppiItem);

        QTableWidgetItem *ppcpmItem = new QTableWidgetItem(QString::number(item.PPcpm, 'f', 2));
        ppcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMtd->setItem(row, 6, ppcpmItem);
    }

    // подгоняем колонки под содержимое
    ui->TableMtd->resizeColumnsToContents();

    // отключаем горизонтальный скролл
    ui->TableMtd->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // вычисляем нужную ширину таблицы
    int width = ui->TableMtd->verticalHeader()->width();
    for (int i = 0; i < ui->TableMtd->columnCount(); ++i)
        width += ui->TableMtd->columnWidth(i);

    // добавляем ширину рамки
    width += ui->TableMtd->frameWidth() * 2;

    ui->TableMtd->setFixedWidth(width);

    ui->TableMtd->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setDataMts(const std::vector<Mts>& data)
{
    QSignalBlocker blocker(ui->TableMts);  // блокируем все сигналы
    ui->TableMts->clear();
    ui->TableMts->setRowCount(data.size());
    ui->TableMts->setColumnCount(7);

    QStringList headers;
    headers << "h" << "w" << "aw" << "TTi" << "TTcpm" << "PPi" << "PPcpm";
    ui->TableMts->setHorizontalHeaderLabels(headers);

    // Убираем любое текущее выделение
    ui->TableMts->setCurrentCell(-1, -1);
    ui->TableMts->clearSelection();

    for (int row = 0; row < data.size(); ++row)
    {
        const Mts &item = data[row];
        QTableWidgetItem *hItem = new QTableWidgetItem(QString::number(item.h, 'f', 2));
        hItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 0, hItem);

        QTableWidgetItem *wItem = new QTableWidgetItem(QString::number(item.w, 'f', 2));
        wItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 1, wItem);

        QTableWidgetItem *awItem = new QTableWidgetItem(QString::number(item.aw, 'f', 2));
        awItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 2, awItem);

        QTableWidgetItem *ttiItem = new QTableWidgetItem(QString::number(item.TTi, 'f', 2));
        ttiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 3, ttiItem);

        QTableWidgetItem *ttcpmItem = new QTableWidgetItem(QString::number(item.TTcpm, 'f', 2));
        ttcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 4, ttcpmItem);

        QTableWidgetItem *ppiItem = new QTableWidgetItem(QString::number(item.PPi, 'f', 2));
        ppiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 5, ppiItem);

        QTableWidgetItem *ppcpmItem = new QTableWidgetItem(QString::number(item.PPcpm, 'f', 2));
        ppcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->TableMts->setItem(row, 6, ppcpmItem);
    }

    // подгоняем колонки под содержимое
    ui->TableMts->resizeColumnsToContents();

    // отключаем горизонтальный скролл
    ui->TableMts->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // вычисляем нужную ширину таблицы
    int width = ui->TableMts->verticalHeader()->width();
    for (int i = 0; i < ui->TableMts->columnCount(); ++i)
        width += ui->TableMts->columnWidth(i);

    // добавляем ширину рамки
    width += ui->TableMts->frameWidth() * 2;

    ui->TableMts->setFixedWidth(width);
    ui->TableMts->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_pushButtonLoadMtd_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть бюллетень Метео",
        "",
        "Text files (*.txt)");

    if (fileName.isEmpty())
        return;

    ui->lineEditMtd->setText(fileName);

    FileParser parser;

    bull_mtd.clear();

    if (!parser.parseTxtFile(fileName, bull_mtd))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    qDebug() << "Размер вектора:" << bull_mtd.size();

    for (const auto& item : bull_mtd)
    {
        qDebug() << "h:" << item.h
                 << "PPi:" << item.PPi
                 << "TTi:" << item.TTi
                 << "av:" << item.av
                 << "v:" << item.v;
    }
}


void MainWindow::on_pushButtonLoad_clicked() // обработка логов для ветра
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть log-файл температуры",
        "",
        "CSV files (*.csv)");

    if (fileName.isEmpty())
        return;

    windLogFilePath = fileName;

    ui->lineEditWind->setText(windLogFilePath);

    QMessageBox::information(this, "Файл загружен","Файл успешно выбран.");
}


void MainWindow::on_pushButtonLoadTempLog_clicked() // обработка логов для температуры
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть log-файл температуры",
        "",
        "CSV files (*.csv)");

    if (fileName.isEmpty())
        return;

    tempLogFilePath = fileName;

    ui->lineEditTemp->setText(tempLogFilePath);

    QMessageBox::information(this, "Файл загружен","Файл успешно выбран.");
}

void MainWindow::on_Button_go_to_start_clicked(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_Button_go_to_table_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButtonCalculateTemp_clicked()
{
    // Очищаем все векторы перед новым расчетом
    coordinates.clear();
    zones.clear();
    mtd.clear();
    mts.clear();
    records.clear();  // если records является членом класса

    // Или используйте reserve только после очистки
    coordinates.reserve(10000);
    zones.reserve(300);
    mtd.reserve(30);
    mts.reserve(30);

    Zone firstZone(0.0);
    Mtd firstMtd(4.0);

    // уровни МДТ
    const std::vector<double> heights = {
        25, 75, 150, 300, 500, 700, 900,
        1100, 1400, 1800, 2200, 2700,
        3500, 4500, 5500, 7000, 9000,
        11000, 13000, 16000, 20000,
        24000, 28000
    };

    //заполнение МТД
    for (double h : heights)
        mtd.emplace_back(h);

    // уровни МТС
    const std::vector<double> h_mts = {
        200, 400, 800, 1200, 1600,
        2000, 2400, 3000, 4000, 5000,
        6000, 8000, 10000, 12000, 14000,
        18000, 22000, 26000, 30000
    };

    //заполнение МТС
    for (double h : h_mts)
        mts.emplace_back(h);

    Analyzer analyzer;
    FileParser parser;

    if (!windLogFilePath.isEmpty())
    {
        if (!parser.parseCSV(windLogFilePath, coordinates, firstZone, firstMtd))
        {
            QMessageBox::critical(this,
                                  "Ошибка",
                                  "Не удалось открыть или прочитать лог ветра.");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Сначала загрузите лог-файл.");
        return;
    }

    zones.push_back(firstZone);

    // Обновляем первый уровень МДТ (вместо добавления нового)
    if (!mtd.empty())
        mtd[0] = firstMtd;
    else
        mtd.push_back(firstMtd);

    zones.reserve(300);

    analyzer.createZones(zones, coordinates);

    analyzer.calculateVk(zones);
    analyzer.calculateVi(zones, mtd);
    analyzer.calculateV(mtd);
    analyzer.calculateDHmtd(mtd);
    analyzer.calculateDHmts(mts);
    analyzer.calculateVm(zones, mts);
    analyzer.calculateWm(mts);

    analyzer.createBullutin(mtd);
    analyzer.createBullutinMts(mts);

    // ТЕМПЕРАТУРА !!!!!!!!!!!!
    globalParam.A = ui->doubleSpinBoxA->value();
    globalParam.B = ui->doubleSpinBoxB->value();
    globalParam.C = ui->doubleSpinBoxC->value();
    globalParam.R1 = ui->doubleSpinBoxR1->value();
    globalParam.R2 = ui->doubleSpinBoxR2->value();

    globalParam.T0 = 10.0;
    globalParam.U0 = 51.0;
    globalParam.P0 = 993.331;

    qDebug("A=%.2f B=%.2f C=%.2f R1=%.2f R2=%.2f", globalParam.A, globalParam.B, globalParam.C, globalParam.R1, globalParam.R2);

    if (tempLogFilePath.isEmpty())
    {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Сначала загрузите log-файл.");
        return;
    }

    // Пытаемся распарсить CSV
    if (!parser.parseTemperatureCSV(tempLogFilePath, records))
    {
        QMessageBox::critical(this,
                              "Ошибка",
                              "Не удалось открыть или прочитать CSV файл.");
        return;
    }

    const std::array<double, 51> Tvir = {
        0.3, 0.3, 0.4, 0.4, 0.4,
        0.5, 0.5, 0.6, 0.6, 0.7,
        0.7, 0.8, 0.8, 0.8, 0.9,
        0.9, 1.0, 1.0, 1.1, 1.2,
        1.3, 1.4, 1.5, 1.6, 1.7,
        1.8, 1.9, 2.0, 2.2, 2.3,
        2.4, 2.55, 2.7, 2.9, 3.1,
        3.3, 3.5, 3.7, 3.9, 4.15,
        4.4, 4.65, 4.9, 5.2, 5.5,
        5.8, 6.1, 6.4, 6.7, 7.05,
        7.4
    };

    std::map<double, double> temperatureTable = {
        {25.0,  15.75},
        {50.0,  15.6},
        {75.0,  15.45},
        {150.0,  14.95},
        {200.0,  15.3},
        {400.0,  14.0},
        {500.0,  12.7},
        {700.0,  11.4},
        {800.0,  12.1},
        {900.0, 10.2},
        {1100.0, 9.0},
        {1200.0, 9.6},
        {1600.0, 7.0},
        {2000.0, 4.5},
        {2400.0, 2.0},
        {3000.0, -1.2},
        {4000.0, -6.2},
        {5000.0, -12.6},
        {6000.0, -18.9},
        {8000.0, -28.4},
        {10000.0, -41.1},
        {12000.0, -50.4},
        {14000.0, -51.5},
        {16000.0, -51.5},
        {18000.0, -51.5},
        {20000.0, -51.5},
    };

    std::map<double, double> DensityTable = {
        {50.0, 1.2},
        {75.0, 1.197},
        {150.0, 1.188},
        {500.0, 1.149},
        {700.0, 1.127},
        {900.0, 1.105},
        {1100.0, 1.108},
    };

    // 0. Считаем толщину зоны
    analyzer.calculateDeltaH(zones);

    // 1. Считаем температуру для каждого измерения
    analyzer.calculateT(records, globalParam);

    // 2. Прибавляем радиационную поправку
    analyzer.addRadio(records);

    // 3. Температура для каждой зоны
    analyzer.calculateTn(records, zones);

    // 4. Вычисляем среднюю высоту зоны
    analyzer.calculateMediumHeight(zones);

    // 5. Вычисляем виртуальную поправку для каждой зоны
    analyzer.calculateDTvir(zones, globalParam);

    // 6. Прибавляем виртуальную поправку к температурам зон
    analyzer.addVir(zones);

    // 7. Вычисляем табличное значение температуры для средних высот зон
    analyzer.fillTabTemperature(temperatureTable, zones);

    // 8. Вычитаем табличное значение, получаем TTi
    analyzer.calculateTTi(zones);

    // 9. Вычисляем TTcpm для зон
    analyzer.calculateTTcpm(zones);

    // 10. Интерполяция, получаем значения для "метеодействительного"
    analyzer.interpolateTemperatureToBullutin(zones, mtd);
    analyzer.interpolateTemperatureToBullutin(zones, mts);

    // Давление и плотность
    // 1. Расчет табличной плотности для каждой зоны
    analyzer.fillTabDensity(DensityTable, zones);

    // 2. Расчет давления в зоне
    analyzer.calculatePn(zones, globalParam);
    analyzer.calculatePi(zones);
    analyzer.calculatePPi(zones);
    analyzer.calculatePPcpm(zones);

    // Интерполяция плотности, получаем значения для "метеодействительного"
    analyzer.interpolateDensityToBullutin(zones, mtd);
    analyzer.interpolateDensityToBullutin(zones, mts);

    //расчет вертикальной устойчивости для зон
    analyzer.calculateTforR(zones);

    // Отладочный вывод (оставьте как есть)
    qDebug() << "---температура для каждой точки---";
    for (const auto& r : records)
    {
        qDebug("QO=%.2f QT=%.2f dtp=%.2f dtv=%.2f Yt=%.2f Rt=%.2f T=%.2f index=%d", r.QO, r.QT, r.dtp, r.dtv, r.Yt, r.Rt, r.T, r.index);
    }

    qDebug() << "---разбивка по зонам---";
    for (const auto& T : zones)
    {
        qDebug("height=%.2f Tn=%.2f Hi=%.2f dTvir=%.2f Tvrn=%.2f Ttab=%.2f TTi=%.2f TTcpm=%.2f", T.height, T.Tn, T.Hi, T.dTvir, T.Tvrn, T.Ttab, T.TTi , T.TTcpm);
    }

    qDebug() << "Интерполяция: метеодействительный";
    for (const auto& i : mtd)
    {
        qDebug("h=%.2f TTi=%.2f TTcpm=%.2f PPi=%.2f PPcpm=%.2f", i.h, i.TTi, i.TTcpm, i.PPi, i.PPcpm);
    }

    qDebug() << "Интерполяция: метеосредний";
    for (const auto& i : mts)
    {
        qDebug("h=%.2f TTi=%.2f TTcpm=%.2f PPi=%.2f PPcpm=%.2f", i.h, i.TTi, i.TTcpm, i.PPi, i.PPcpm);
    }

    qDebug() << "Давление и плотность";
    for (const auto& T : zones)
    {
        qDebug("height=%.2f Pn=%.2f Pi=%.2f PPi=%.2f Pitab=%.2f PPcpm=%.2f",T.height, T.Pn, T.Pi, T.PPi, T.Pitab, T.PPcpm);
    }

    qDebug() << "Вертикальная устойчивость по зонам";
    for (const auto& T : zones)
    {
        qDebug("height=%.2f T=%.2f Ri=%.2f",T.height, T.T, T.Ri);
    }

    // Обновляем таблицы новыми данными
    setDataMtd(mtd);
    setDataMts(mts);
    setDataTableMtd(mtd);

    // Переходим на страницу с таблицами
    ui->stackedWidget->setCurrentIndex(3);
}
