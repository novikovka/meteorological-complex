#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "fileparser.h"
#include "analyzer.h"
#include "types.h"

#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть CSV файл",
        "",
        "CSV files (*.csv)");

    if (fileName.isEmpty())
        return;

    // создаем все нужные вектора
    std::vector<Coordinate> coordinates;
    std::vector<Zone> zones;
    std::vector<Mtd> mtd;
    std::vector<Mts> mts;

    coordinates.reserve(10000);
    zones.reserve(300);
    mtd.reserve(30);
    mts.reserve(30);

    // приземка
    Zone firstZone(0.0);
    Mtd firstMtd(4.0);

    //создаем парсер
    FileParser parser;

    if (!parser.parseCSV(fileName, coordinates, firstZone, firstMtd))
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    // добавляем первую зону и первый MTD
    zones.push_back(firstZone);
    mtd.push_back(firstMtd);

    // вывод координат в интерфейс
    ui->listWidgetX->clear();
    for (const auto& c : coordinates)
        ui->listWidgetX->addItem(c.toString());


    Analyzer analyzer;

    analyzer.createZones(zones, coordinates);

    // добавляем уровни МДТ
    std::vector<double> heights = {
        25, 75, 150, 300, 500, 700, 900,
        1100, 1400, 1800, 2200, 2700,
        3500, 4500, 5500, 7000, 9000,
        11000, 13000, 16000, 20000,
        24000, 28000
    };

    for (double h : heights)
        mtd.emplace_back(h);

    // добавляем уровни МТС (метеосредний)
    std::vector<double> h_mts = {
        200, 400, 800, 1200, 1600,
        2000, 2400, 3000, 4000, 5000,
        6000, 8000, 10000, 12000, 14000,
        18000, 22000, 26000, 30000
    };


    for (double h : h_mts)
        mts.emplace_back(h);

    analyzer.calculateVk(zones);
    analyzer.calculateVi(zones, mtd);
    analyzer.calculateV(mtd);
    //void calculateDHmtd(std::vector<Mtd>& mtd);
    analyzer.calculateDHmtd(mtd);
    analyzer.calculateDHmts(mts);

    analyzer.calculateVm(zones, mts);

    analyzer.calculateWm(mts);


    qDebug() << "------зоны------";
    for (const auto& z : zones)
    {
        qDebug("H=%.2f  x=%.2f  z=%.2f  s=%.2f  vx=%.2f  vz=%.2f  y=%.2f dh=%.2f",
                z.height, z.x, z.z, z.s, z.vx, z.vz, z.y, z.dh);
    }

    qDebug() << "---метеодействительный---";

    for (const auto& m : mtd)
    {
        qDebug("H=%.2f vx=%.2f vz=%.2f v=%.2f av=%.2f dh=%.2f", m.h, m.vx, m.vz, m.v, m.av, m.dh);
    }

    qDebug() << "---метеосредний---";

    for (const auto& m : mts)
    {
        qDebug("H=%.2f vx=%.2f vz=%.2f wx=%.2f wz=%.2f w=%.2f aw=%.2f dh=%.2f", m.h, m.vx, m.vz, m.wx, m.wz, m.w, m.aw, m.dh);
    }

    qDebug() << "---метеодействительный с округлением---";
    analyzer.createBullutin(mtd);

    qDebug() << "---метеосредний с округлением---";
    analyzer.createBullutinMts(mts);
}

void MainWindow::on_pushButtonLoadTempLog_clicked()
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
    Analyzer analyzer;

    UserConstants globalParam;

    globalParam.A = ui->doubleSpinBoxA->value();
    globalParam.B = ui->doubleSpinBoxB->value();
    globalParam.C = ui->doubleSpinBoxC->value();
    globalParam.R1 = ui->doubleSpinBoxR1->value();
    globalParam.R2 = ui->doubleSpinBoxR2->value();

    qDebug("A=%.2f B=%.2f C=%.2f R1=%.2f R2=%.2f", globalParam.A, globalParam.B, globalParam.C, globalParam.R1, globalParam.R2);

    std::vector<Tzone> Tzones;
    Tzones.reserve(300);

    analyzer.createTzones(Tzones);


    if (tempLogFilePath.isEmpty())
    {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Сначала загрузите log-файл.");
        return;
    }

    // Создаём объект парсера
    FileParser parser;

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

    analyzer.calculateT(records, globalParam);
    analyzer.calculateTn(records, Tzones);
    analyzer.calculateMediumHeight(Tzones);

    qDebug() << "---температура для каждой точки---";

    for (const auto& r : records)
    {
        qDebug("QO=%.2f QT=%.2f dtp=%.2f Yt=%.2f Rt=%.2f T=%.2f index=%d", r.QO, r.QT, r.dtp, r.Yt, r.Rt, r.T, r.index);
    }

    for (const auto& T : Tzones)
    {
        qDebug("height=%.2f Tn=%.2f Hi=%.2f", T.height, T.Tn, T.Hi);
    }

}


