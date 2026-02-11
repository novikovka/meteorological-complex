#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include <vector>
#include <cmath>
#include <limits>

using namespace std;

constexpr double KDU = M_PI / 3000.0;
constexpr double EPS = 1e-9;

struct Coordinate {
    double X;
    double Z;
    double H;
    double S;

    QString toString() const {
        return QString("X=%1 Z=%2 H=%3 S=%4")
            .arg(X, 0, 'f', 3)
            .arg(Z, 0, 'f', 3)
            .arg(H, 0, 'f', 3)
            .arg(S, 0, 'f', 3);
    }
};

class Zone {
public:
    double height{};
    double x;
    double z;
    double s;
    double vx;
    double vz;

    Zone(double h, const std::vector<Coordinate>& coords): height(h){

        const Coordinate* closest = findClosest(h, coords);
        if (closest) {
            x = closest->X;
            z = closest->Z;
            s = closest->S;
        }
    }

private:
    static const Coordinate* findClosest(double h,const std::vector<Coordinate>& coords){
        if (coords.empty())
            return nullptr;

        const Coordinate* result = &coords[0];
        double minDiff = std::abs(coords[0].H - h);

        for (const auto& c : coords) {
            double diff = std::abs(c.H - h);
            if (diff < minDiff) {
                minDiff = diff;
                result = &c;
            }
        }

        return result;
    }
};

static void calculateV(std::vector<Zone>& zones)
{
    zones[0].vx = 0.0;
    zones[0].vz = 0.0;

    for (size_t i = 1; i < zones.size(); ++i)
    {
        double dt = zones[i].s - zones[i - 1].s;

        if (abs(dt) < EPS) {
            zones[i].vx = 0.0;
            zones[i].vz = 0.0;
            continue;
        }

        zones[i].vx = (zones[i].x - zones[i - 1].x) / dt;
        zones[i].vz = (zones[i].z - zones[i - 1].z) / dt;
    }
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButtonLoad_clicked(){
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть CSV файл",
        "",
        "CSV files (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    ui->listWidgetX->clear();

    QTextStream in(&file);

    std::vector<Coordinate> coordinates;
    coordinates.reserve(10000); // можно изменить при необходимости

    // чтение csv
    while (!in.atEnd())
    {
        const QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        const QStringList values = line.split(',');
        if (values.size() < 4)
            continue;

        const double dglob = values[0].toDouble();
        const double aglob = values[1].toDouble();
        const double eglob = values[2].toDouble();
        const double tglob = values[3].toDouble();

        const double cosE = cos(eglob * KDU);
        const double sinE = sin(eglob * KDU);
        const double cosA = cos(aglob * KDU);
        const double sinA = sin(aglob * KDU);

        const double X = dglob * cosE * cosA;
        const double Z = dglob * cosE * sinA;
        const double H = dglob * sinE + 0.6868e-7 * pow(dglob * cosE, 2);
        const double S = tglob;

        Coordinate coord{X, Z, H, S};
        coordinates.emplace_back(coord);

        // выводим в интерфейс координаты
        ui->listWidgetX->addItem(coord.toString());
    }

    file.close();

    // создание зон
    vector<Zone> zones;
    zones.reserve(200); // не знаю сколько точно места надо

    auto addZones = [&](double from, double to, double step)
    {
        for (double h = from; h < to; h += step)
            zones.emplace_back(h, coordinates);
    };

    addZones(0.0,     500.0,   100.0);
    addZones(600.0,   6000.0,  200.0);
    addZones(6000.0,  14000.0, 400.0);
    addZones(14000.0, 20000.0, 500.0);

    // составляющие скорости
    calculateV(zones);

    //проверка зон
    for (const auto& zone : zones){
        qDebug("Zone H=%.2f  x=%.2f  z=%.2f  s=%.2f  vx=%.2f  vz=%.2f",
               zone.height,
               zone.x,
               zone.z,
               zone.s,
               zone.vx,
               zone.vz);
    }
}

