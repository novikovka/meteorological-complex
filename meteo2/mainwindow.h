#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <vector>

#include "types.h"   // Coordinate, Zone, Mtd, Mts

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Вкладка "Ветер"
    void on_pushButtonLoad_clicked();

    // Вкладка "Температура"
    void on_pushButtonLoadTempLog_clicked();
    void on_pushButtonCalculateTemp_clicked();

private:
    Ui::MainWindow *ui;

    // -----------------------------
    // ДАННЫЕ ПРОЕКТА (состояние окна)
    // -----------------------------

    std::vector<Coordinate> coordinates;
    std::vector<Zone> zones;
    std::vector<Mtd> mtd;
    std::vector<Mts> mts;

    // путь к температурному лог-файлу
    QString tempLogFilePath;

    // создание уровней бюллетеня
    void initializeBulletinLevels();
};

#endif // MAINWINDOW_H
