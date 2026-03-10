#ifndef RESULTSWINDOW_H
#define RESULTSWINDOW_H

#include <QDialog>
#include "types.h"

namespace Ui {
class resultswindow;
}

class resultswindow : public QDialog
{
    Q_OBJECT

public:
    explicit resultswindow(QWidget *parent = nullptr);
    ~resultswindow();

    void setDataMtd(const std::vector<Mtd>& data);
    void setDataMts(const std::vector<Mts>& data);

private:
    Ui::resultswindow *ui;
};

#endif // RESULTSWINDOW_H
