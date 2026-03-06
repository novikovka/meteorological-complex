#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <unordered_map>

#include "fileparser.h"
#include "analyzer.h"
#include "types.h"

#include <vector>
#include <array>
#include <map>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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

    QMessageBox::information(this, "Файл загружен","Файл успешно выбран.");
}

void MainWindow::on_pushButtonCalculateTemp_clicked()
{
    // Можно заранее зарезервировать память
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
    UserConstants globalParam;
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
    mtd.push_back(firstMtd);

    // Если нужно — обновляем первый уровень МДТ
    if (!mtd.empty())
        mtd[0] = firstMtd;

    // Вывод координат в интерфейс
    ui->listWidgetX->clear();
    for (const auto& c : coordinates)
        ui->listWidgetX->addItem(c.toString());

    //std::vector<Zone> zones;
    zones.reserve(300);

    //analyzer.createTzones(Tzones);

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

    // Создаём объект парсера


    // Вектор для хранения записей температуры
    std::vector<TemperatureRecord> records;

    // Пытаемся распарсить CSV
    if (!parser.parseTemperatureCSV(tempLogFilePath, records))
    {
        QMessageBox::critical(this,
                              "Ошибка",
                              "Не удалось открыть или прочитать CSV файл.");
        return;
    }

    // Очищаем текстовое поле
    ui->textEditTempResult->clear();

    // Выводим все записи
    for (const auto& rec : records)
    {
        ui->textEditTempResult->append(
            QString("QO: %1, QT: %2, dtp: %3")
                .arg(rec.QO)
                .arg(rec.QT)
                .arg(rec.dtp)
            );
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
    analyzer.interpolateTemperatureToBullutin(zones,mtd);

    analyzer.interpolateTemperatureToBullutin(zones,mts);


    // Давление и плотность

    // 1. Расчет табличной плотности для каждой зоны
    analyzer.fillTabDensity(DensityTable,zones);

    // 2. Расчет давления в зоне
    analyzer.calculatePn(zones,globalParam);
    analyzer.calculatePi(zones);
    analyzer.calculatePPi(zones);
    analyzer.calculatePPcpm(zones);

    // Интерполяция плотности, получаем значения для "метеодействительного"
    analyzer.interpolateDensityToBullutin(zones,mtd);

    analyzer.interpolateDensityToBullutin(zones,mts);

    //расчет вертикальной устойчивости
    analyzer.calculateTforR(zones);

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

}


