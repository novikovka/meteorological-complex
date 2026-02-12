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

class Mtd {
public:
    double h;
    double y_prev;
    double y_next;
    double vx;
    double vz;
    double v;
    double av;

    Mtd(double h = 0.0, double y_prev = 0.0, double y_next = 0.0,double vx = 0.0, double vz = 0.0, double v = 0.0, double av = 0.0)
        : h(h), y_prev(y_prev), y_next(y_next),vx(vx), vz(vz), v(v), av(av) {}
};

class Zone {
public:
    double height;
    double x;
    double z;
    double s;
    double vx;
    double vz;
    double dh;
    double y;

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

static void calculateVk(vector<Zone>& zones)
{
    zones[0].vx = 0.0;
    zones[0].vz = 0.0;
    zones[0].dh = 0.0;
    zones[0].y = 0.0;

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
        zones[i].dh = (zones[i].height - zones[i - 1].height) / 2;
        zones[i].y = (zones[i].height + zones[i - 1].height) / 2;

    }
}

static void calculateVi(const vector<Zone>& zones, vector<Mtd>& mtd)
{
    for (auto& m : mtd)
    {
        // перебираем зоны
        for (size_t k = 1; k < zones.size(); ++k)
        {
            double y_prev = zones[k - 1].y; // Yk-1
            double y_next = zones[k].y; // Yk

            if (m.h >= y_prev && m.h <= y_next) // если высота из бюллетеня попадает в текущаю зону
            {
                // сохраняем границы интервалов для проверки
                m.y_prev = y_prev;
                m.y_next = y_next;

                double vx_prev = zones[k - 1].vx; // VXk-1
                double vx_next = zones[k].vx; // VXk

                double vz_prev = zones[k - 1].vz; //VZk-1
                double vz_next = zones[k].vz; //Vzk

                double dy = y_next - y_prev; // знаменатель
                double dvx = vx_next - vx_prev;
                double dvz = vz_next - vz_prev;

                m.vx = vx_prev + (dvx / dy ) * (m.h - y_prev);
                m.vz = vz_prev + (dvz / dy ) * (m.h - y_prev);

                break;
            }
        }
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

    vector<Coordinate> coordinates;
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
    zones.reserve(300); // не знаю сколько точно места надо

    auto addZones = [&](double from, double to, double step)
    {
        for (double h = from; h < to; h += step)
            zones.emplace_back(h, coordinates);
    };

    addZones(0.0,     500.0,   100.0);
    addZones(600.0,   6000.0,  200.0);
    addZones(6000.0,  14000.0, 400.0);
    addZones(14000.0, 20000.0, 500.0);

    // составляющие скорости для каждой зоны
    calculateVk(zones);

    //проверка зон
    for (const auto& zone : zones){
        qDebug("Zone H=%.2f  x=%.2f  z=%.2f  s=%.2f  vx=%.2f  vz=%.2f dh=%.2f  y=%.2f",
               zone.height,
               zone.x,
               zone.z,
               zone.s,
               zone.vx,
               zone.vz,
               zone.dh,
               zone.y);
    }

    /*
    vector<Mtd> mtd;
    mtd.emplace_back(4.0);
    mtd.emplace_back(25.0, 0, 50.0);
    mtd.emplace_back(75.0, 50.0, 150.0);
    mtd.emplace_back(150.0, 150.0, 250);
    mtd.emplace_back(300.0, 250.0, 350.0);
    mtd.emplace_back(500.0, 350.0, 500.0);
    mtd.emplace_back(700.0, 500.0, 700.0);
    mtd.emplace_back(900.0, 700.0, 900.0);
    mtd.emplace_back(1100.0, 900.0, 1100.0);
    mtd.emplace_back(1400.0, 1130.0, 1500.0);
    mtd.emplace_back(1800.0, 1700.0, 1900.0);
    mtd.emplace_back(2200.0, 2100.0, 2300.0);
    mtd.emplace_back(2700.0, 2500.0, 2700.0);
    mtd.emplace_back(3500.0, 3300.0, 3500.0);
    mtd.emplace_back(4500.0, 4300.0, 4500.0);
    mtd.emplace_back(5500.0, 5300.0, 5500.0);
    mtd.emplace_back(7000.0, 6600.0, 7000.0);
    mtd.emplace_back(9000.0, 8600.0, 9000.0);
    mtd.emplace_back(11000.0, 10600.0, 11000.0);
    mtd.emplace_back(13000.0, 12600.0, 13000.0);
    mtd.emplace_back(16000.0, 15750.0, 16250.0);
    mtd.emplace_back(20000.0, 19500.0, 20000.0);
    mtd.emplace_back(24000.0, 24000.0, 23500.0);
    mtd.emplace_back(28000.0, 27500.0, 28000.0);
*/
    vector<Mtd> mtd;
    mtd.emplace_back(4.0);
    mtd.emplace_back(25.0);
    mtd.emplace_back(75.0);
    mtd.emplace_back(150.0);
    mtd.emplace_back(300.0);
    mtd.emplace_back(500.0);
    mtd.emplace_back(700.0);
    mtd.emplace_back(900.0);
    mtd.emplace_back(1100.0);
    mtd.emplace_back(1400.0);
    mtd.emplace_back(1800.0);
    mtd.emplace_back(2200.0);
    mtd.emplace_back(2700.0);
    mtd.emplace_back(3500.0);
    mtd.emplace_back(4500.0);
    mtd.emplace_back(5500.0);
    mtd.emplace_back(7000.0);
    mtd.emplace_back(9000.0);
    mtd.emplace_back(11000.0);
    mtd.emplace_back(13000.0);
    mtd.emplace_back(16000.0);
    mtd.emplace_back(20000.0);
    mtd.emplace_back(24000.0);
    mtd.emplace_back(28000.0);

    // составляющие скорости для "Метеодействительный"
    calculateVi(zones, mtd);

    //проверка бюллетеня "Метеодействительный"
    for (const auto& cur : mtd){
        qDebug("H=%.2f y_prev=%.2f y_next=%.2f vx=%.2f  vz=%.2f",
               cur.h, cur.y_prev, cur.y_next, cur.vx, cur.vz);
    }

}




