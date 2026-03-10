#include "resultswindow.h"
#include "ui_resultswindow.h"

#include "types.h"

resultswindow::resultswindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::resultswindow)
{
    ui->setupUi(this);
}

resultswindow::~resultswindow()
{
    delete ui;
}

/*
void resultswindow::setData(const std::vector<Mtd>& data)
{
    QString report;

    report += QString("%1 %2 %3 %4 %5 %6 %7\n")
                  .arg("h",10)
                  .arg("v",10)
                  .arg("av",10)
                  .arg("TTi",10)
                  .arg("TTcpm",10)
                  .arg("PPi",10)
                  .arg("PPcpm",10);

    report += QString(80,'-') + "\n";

    for(const Mtd &row : data)
    {
        report += QString("%1 %2 %3 %4 %5 %6 %7\n")
        .arg(row.h,10,'f',2)
            .arg(row.v,10,'f',2)
            .arg(row.av,10,'f',2)
            .arg(row.TTi,10,'f',2)
            .arg(row.TTcpm,10,'f',2)
            .arg(row.PPi,10,'f',2)
            .arg(row.PPcpm,10,'f',2);
    }

    ui->reportText->setPlainText(report);
}

*/
/*
void resultswindow::setDataMtd(const std::vector<Mtd>& data)
{
    // Очищаем таблицу
    ui->TableMtd->clear();

    // Устанавливаем количество строк и столбцов
    ui->TableMtd->setRowCount(data.size());
    ui->TableMtd->setColumnCount(7);

    // Устанавливаем заголовки столбцов
    QStringList headers;
    headers << "h" << "v" << "av" << "TTi" << "TTcpm" << "PPi" << "PPcpm";
    ui->TableMtd->setHorizontalHeaderLabels(headers);

    // Заполняем таблицу данными
    for (int row = 0; row < data.size(); ++row)
    {
        const Mtd &item = data[row];

        // Создаем элементы для каждой ячейки с форматированием чисел
        QTableWidgetItem *hItem = new QTableWidgetItem(QString::number(item.h, 'f', 2));
        QTableWidgetItem *vItem = new QTableWidgetItem(QString::number(item.v, 'f', 2));
        QTableWidgetItem *avItem = new QTableWidgetItem(QString::number(item.av, 'f', 2));
        QTableWidgetItem *ttiItem = new QTableWidgetItem(QString::number(item.TTi, 'f', 2));
        QTableWidgetItem *ttcpmItem = new QTableWidgetItem(QString::number(item.TTcpm, 'f', 2));
        QTableWidgetItem *ppiItem = new QTableWidgetItem(QString::number(item.PPi, 'f', 2));
        QTableWidgetItem *ppcpmItem = new QTableWidgetItem(QString::number(item.PPcpm, 'f', 2));

        // Устанавливаем выравнивание по правому краю для чисел
        hItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        vItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        avItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ttiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ttcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ppiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ppcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Устанавливаем элементы в таблицу
        ui->TableMtd->setItem(row, 0, hItem);
        ui->TableMtd->setItem(row, 1, vItem);
        ui->TableMtd->setItem(row, 2, avItem);
        ui->TableMtd->setItem(row, 3, ttiItem);
        ui->TableMtd->setItem(row, 4, ttcpmItem);
        ui->TableMtd->setItem(row, 5, ppiItem);
        ui->TableMtd->setItem(row, 6, ppcpmItem);
    }

    // Автоматически подгоняем ширину столбцов под содержимое
    ui->TableMtd->resizeColumnsToContents();

    // Добавляем возможность сортировки (опционально)
    ui->TableMtd->setSortingEnabled(true);

    // Устанавливаем политику выделения (опционально)
    ui->TableMtd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableMtd->setSelectionMode(QAbstractItemView::SingleSelection);
}

void resultswindow::setDataMts(const std::vector<Mts>& data)
{
    // Очищаем таблицу
    ui->TableMts->clear();

    // Устанавливаем количество строк и столбцов
    ui->TableMts->setRowCount(data.size());
    ui->TableMts->setColumnCount(7);

    // Устанавливаем заголовки столбцов
    QStringList headers;
    headers << "h" << "w" << "aw" << "TTi" << "TTcpm" << "PPi" << "PPcpm";
    ui->TableMts->setHorizontalHeaderLabels(headers);

    // Заполняем таблицу данными
    for (int row = 0; row < data.size(); ++row)
    {
        const Mts &item = data[row];

        // Создаем элементы для каждой ячейки с форматированием чисел
        QTableWidgetItem *hItem = new QTableWidgetItem(QString::number(item.h, 'f', 2));
        QTableWidgetItem *vItem = new QTableWidgetItem(QString::number(item.w, 'f', 2));
        QTableWidgetItem *avItem = new QTableWidgetItem(QString::number(item.aw, 'f', 2));
        QTableWidgetItem *ttiItem = new QTableWidgetItem(QString::number(item.TTi, 'f', 2));
        QTableWidgetItem *ttcpmItem = new QTableWidgetItem(QString::number(item.TTcpm, 'f', 2));
        QTableWidgetItem *ppiItem = new QTableWidgetItem(QString::number(item.PPi, 'f', 2));
        QTableWidgetItem *ppcpmItem = new QTableWidgetItem(QString::number(item.PPcpm, 'f', 2));

        // Устанавливаем выравнивание по правому краю для чисел
        hItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        vItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        avItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ttiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ttcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ppiItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ppcpmItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Устанавливаем элементы в таблицу
        ui->TableMts->setItem(row, 0, hItem);
        ui->TableMts->setItem(row, 1, vItem);
        ui->TableMts->setItem(row, 2, avItem);
        ui->TableMts->setItem(row, 3, ttiItem);
        ui->TableMts->setItem(row, 4, ttcpmItem);
        ui->TableMts->setItem(row, 5, ppiItem);
        ui->TableMts->setItem(row, 6, ppcpmItem);
    }

    // Автоматически подгоняем ширину столбцов под содержимое
    ui->TableMts->resizeColumnsToContents();

    // Добавляем возможность сортировки (опционально)
    ui->TableMts->setSortingEnabled(true);

    // Устанавливаем политику выделения (опционально)
    ui->TableMts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableMts->setSelectionMode(QAbstractItemView::SingleSelection);
}
*/

/*
сейчас у меня есть виджет типа QPlainTextEdit и названием reportText, данные в нем заполняются с помощью этого метода:
    перепиши этот метод таким образом чтобы с его помощью заполнялся виджет типа QTableWidget и названием  TableMtd
*/
