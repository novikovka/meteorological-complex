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

    coordinates.reserve(10000);
    zones.reserve(300);
    mtd.reserve(30);

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

    analyzer.calculateVk(zones);
    analyzer.calculateVi(zones, mtd);
    analyzer.calculateV(mtd);




    qDebug() << "------зоны------";
    for (const auto& z : zones)
    {
        qDebug("H=%.2f  x=%.2f  z=%.2f  s=%.2f  vx=%.2f  vz=%.2f  y=%.2f",
                z.height, z.x, z.z, z.s, z.vx, z.vz, z.y);
    }

    qDebug() << "---метеодействительный---";

    for (const auto& m : mtd)
    {
        qDebug("H=%.2f vx=%.2f vz=%.2f v=%.2f av=%.2f", m.h, m.vx, m.vz, m.v, m.av);
    }

    qDebug() << "---метеодействительный с округлением---";
    analyzer.createBullutin(mtd);
}
