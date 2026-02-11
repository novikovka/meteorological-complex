#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <cmath>
#include <vector>

#define KDU (M_PI/3000.0)

class Zone {
public:
    double zoneHeight; // порог высоты
    double vx;
    double vz;

    Zone(double h, double vx_val = 0.0, double vz_val = 0.0)
        : zoneHeight(h), vx(vx_val), vz(vz_val) {}
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
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
        this, "Открыть CSV файл", "", "CSV files (*.csv)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    ui->listWidgetX->clear();

    QTextStream in(&file); //поток для чтения текста из файла

    //const double PI = 3.14159265358979323846;

    // Создание зон
    std::vector<Zone> zones;

    // До 500 м шаг 100 м
    for (double h = 0.0; h <= 500.0-1; h += 100.0)
        zones.emplace_back(h);

    // 500–6000 м шаг 200 м
    for (double h = 600.0; h <= 6000.0-1; h += 200.0)
        zones.emplace_back(h);

    // 6000–14000 м шаг 400 м
    for (double h = 6000.0; h <= 14000.0-1; h += 400.0)
        zones.emplace_back(h);

    // 14000+ м шаг 800 м
    for (double h = 14000.0; h <= 20000.0-1; h += 800.0)
        zones.emplace_back(h);

    // читаем логи
    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.trimmed().isEmpty())
            continue;

        QStringList values = line.split(',');

        if (values.size() < 4)
            continue;

        // Преобразуем в double
        double dglob = values[0].trimmed().toDouble();
        double aglob = values[1].trimmed().toDouble();
        double eglob = values[2].trimmed().toDouble();
        // double tglob = values[3];

        double X = dglob * cos(eglob * KDU) * cos(aglob * KDU);
        double Z = dglob * cos(eglob * KDU) * sin(aglob * KDU);
        double H = dglob * sin(eglob * KDU) + 0.6868 * (1e-7) * pow(dglob * cos(eglob * KDU), 2);

        // строка для отображения
        QString displayText = QString("X = %1, Z = %2, H = %3")
                                  .arg(X, 0, 'f', 3)
                                  .arg(Z, 0, 'f', 3)
                                  .arg(H, 0, 'f', 3);

        ui->listWidgetX->addItem(displayText);
    }

    file.close();

    // Проверка зон
    for (const auto& zone : zones) {
        qDebug("Zone Height: %.2f, vx=%.2f, vz=%.2f", zone.zoneHeight, zone.vx, zone.vz);
    }
}
