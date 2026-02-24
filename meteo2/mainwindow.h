#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

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

    // путь к температурному лог-файлу
    QString tempLogFilePath;
};

#endif // MAINWINDOW_H
