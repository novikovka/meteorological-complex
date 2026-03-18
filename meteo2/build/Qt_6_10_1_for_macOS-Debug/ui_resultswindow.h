/********************************************************************************
** Form generated from reading UI file 'resultswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTSWINDOW_H
#define UI_RESULTSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_resultswindow
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QTableWidget *tableMtd;
    QTableWidget *tableMts;
    QWidget *page_2;

    void setupUi(QDialog *resultswindow)
    {
        if (resultswindow->objectName().isEmpty())
            resultswindow->setObjectName("resultswindow");
        resultswindow->resize(1526, 974);
        stackedWidget = new QStackedWidget(resultswindow);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 20, 1511, 761));
        page = new QWidget();
        page->setObjectName("page");
        tableMtd = new QTableWidget(page);
        tableMtd->setObjectName("tableMtd");
        tableMtd->setGeometry(QRect(10, 20, 581, 601));
        tableMts = new QTableWidget(page);
        tableMts->setObjectName("tableMts");
        tableMts->setGeometry(QRect(630, 20, 671, 661));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        stackedWidget->addWidget(page_2);

        retranslateUi(resultswindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(resultswindow);
    } // setupUi

    void retranslateUi(QDialog *resultswindow)
    {
        resultswindow->setWindowTitle(QCoreApplication::translate("resultswindow", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class resultswindow: public Ui_resultswindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTSWINDOW_H
