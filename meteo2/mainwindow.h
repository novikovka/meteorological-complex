/*
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

// В MainWindow.h объявите структуру: //
struct TableClickInfo {
    QString tableName;
    QString columnName;
    int row;
    int column;
    QDateTime clickTime;

    TableClickInfo() : row(-1), column(-1) {}

    bool isValid() const {
        return !tableName.isEmpty() && !columnName.isEmpty() && row >= 0;
    }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // -----------------------------
    // Вкладка "Ветер"
    // -----------------------------
    void on_pushButtonLoad_clicked();

    // -----------------------------
    // Вкладка "Температура"
    // -----------------------------
    void on_pushButtonLoadTempLog_clicked();
    void on_pushButtonCalculateTemp_clicked();

    // -----------------------------
    // Таблица МТД
    // -----------------------------
    void setDataMtd(const std::vector<Mtd>& data);

    // Клик по таблице МТД
    void onTableMtdClicked(int row, int column);

    //кнопки "назад"

    void on_Button_go_to_start_clicked();
    void on_Button_go_to_table_clicked();

private:

    Ui::MainWindow *ui;

    // -----------------------------
    // ДАННЫЕ ПРОЕКТА (состояние окна)
    // -----------------------------

    std::vector<Coordinate> coordinates;
    std::vector<Zone> zones;
    std::vector<Mtd> mtd;
    std::vector<Mts> mts;

    // путь к ветренному лог-файлу
    QString windLogFilePath;

    // путь к температурному лог-файлу
    QString tempLogFilePath;

    TableClickInfo m_lastClickInfo; //

signals:
    void tableCellClicked(const TableClickInfo& info); //

};

#endif // MAINWINDOW_H
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDateTime>
#include <vector>

#include "types.h"
#include "displaymanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Структура для хранения информации о клике по таблице
struct TableClickInfo {
    QString tableName;
    QString columnName;
    int row;
    int column;
    QDateTime clickTime;
    QString cellValue; // РЕКОМЕНДАЦИЯ: добавить значение ячейки

    TableClickInfo() : row(-1), column(-1) {}

    bool isValid() const {
        return !tableName.isEmpty() && !columnName.isEmpty() && row >= 0;
    }

    // РЕКОМЕНДАЦИЯ: добавить метод для форматированного вывода
    QString toString() const {
        return QString("Таблица: %1, Колонка: %2, Строка: %3, Столбец: %4, Время: %5")
            .arg(tableName).arg(columnName).arg(row).arg(column)
            .arg(clickTime.toString("dd.MM.yyyy hh:mm:ss"));
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // пока что
    TableClickInfo getLastClickInfo() const { return m_lastClickInfo; }

private slots:

    void on_pushButtonLoadMtd_clicked();

    // -----------------------------
    // Вкладка "Ветер"
    // -----------------------------
    void on_pushButtonLoad_clicked();

    // -----------------------------
    // Вкладка "Температура"
    // -----------------------------
    void on_pushButtonLoadTempLog_clicked();
    void on_pushButtonCalculateTemp_clicked();

    // Создание таблиц бюллетеней
    void setDataMtd(const std::vector<Mtd>& data);
    void setDataMts(const std::vector<Mts>& data);

    // новая версия: сравнение с выходными данными
    void setDataTableMtd(const std::vector<Mtd>& data);
    void setDataTableBullMtd(const std::vector<Bull_mtd>& data);

    // Клик по таблице МТД
    void onTableMtdClicked(int row, int column);
    void onTableMtsClicked(int row, int column);

    //сравнение и закрашивание ячеек
    void compareVColumns();

    //для cursor pointer
    void onTableCellEntered(int row, int column);

    // Кнопки "назад"
    void on_Button_go_to_start_clicked();
    void on_Button_go_to_table_clicked();

private:
    Ui::MainWindow *ui;

    std::vector<Coordinate> coordinates;
    std::vector<Zone> zones;
    std::vector<Mtd> mtd;
    std::vector<Mts> mts;
    std::vector<TemperatureRecord> records;
    std::vector<Bull_mtd> bull_mtd;

    //константы, которые пользователь вводит при загрузке логов
    UserConstants globalParam;

    // путь к ветренному лог-файлу
    QString windLogFilePath;

    // путь к температурному лог-файлу
    QString tempLogFilePath;

    TableClickInfo m_lastClickInfo;
    DisplayManager displayManager;

signals:
    void tableCellClicked(const TableClickInfo& info);

};

#endif // MAINWINDOW_H
