/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButtonLoad;
    QListWidget *listWidgetX;
    QWidget *tab_2;
    QPushButton *pushButtonLoadTempLog;
    QDoubleSpinBox *doubleSpinBoxA;
    QDoubleSpinBox *doubleSpinBoxB;
    QDoubleSpinBox *doubleSpinBoxC;
    QDoubleSpinBox *doubleSpinBoxR1;
    QDoubleSpinBox *doubleSpinBoxR2;
    QPushButton *pushButtonCalculateTemp;
    QTextEdit *textEditTempResult;
    QLabel *label;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 791, 611));
        tab = new QWidget();
        tab->setObjectName("tab");
        pushButtonLoad = new QPushButton(tab);
        pushButtonLoad->setObjectName("pushButtonLoad");
        pushButtonLoad->setGeometry(QRect(40, 40, 171, 51));
        listWidgetX = new QListWidget(tab);
        listWidgetX->setObjectName("listWidgetX");
        listWidgetX->setGeometry(QRect(40, 100, 401, 461));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        pushButtonLoadTempLog = new QPushButton(tab_2);
        pushButtonLoadTempLog->setObjectName("pushButtonLoadTempLog");
        pushButtonLoadTempLog->setGeometry(QRect(60, 50, 171, 51));
        doubleSpinBoxA = new QDoubleSpinBox(tab_2);
        doubleSpinBoxA->setObjectName("doubleSpinBoxA");
        doubleSpinBoxA->setGeometry(QRect(60, 120, 131, 31));
        doubleSpinBoxA->setMaximum(100000.000000000000000);
        doubleSpinBoxB = new QDoubleSpinBox(tab_2);
        doubleSpinBoxB->setObjectName("doubleSpinBoxB");
        doubleSpinBoxB->setGeometry(QRect(200, 120, 131, 31));
        doubleSpinBoxB->setMaximum(100000.000000000000000);
        doubleSpinBoxC = new QDoubleSpinBox(tab_2);
        doubleSpinBoxC->setObjectName("doubleSpinBoxC");
        doubleSpinBoxC->setGeometry(QRect(350, 120, 131, 31));
        doubleSpinBoxC->setMaximum(100000.000000000000000);
        doubleSpinBoxR1 = new QDoubleSpinBox(tab_2);
        doubleSpinBoxR1->setObjectName("doubleSpinBoxR1");
        doubleSpinBoxR1->setGeometry(QRect(60, 160, 131, 31));
        doubleSpinBoxR1->setMaximum(100000.000000000000000);
        doubleSpinBoxR2 = new QDoubleSpinBox(tab_2);
        doubleSpinBoxR2->setObjectName("doubleSpinBoxR2");
        doubleSpinBoxR2->setGeometry(QRect(200, 160, 131, 31));
        doubleSpinBoxR2->setMaximum(100000.000000000000000);
        pushButtonCalculateTemp = new QPushButton(tab_2);
        pushButtonCalculateTemp->setObjectName("pushButtonCalculateTemp");
        pushButtonCalculateTemp->setGeometry(QRect(260, 50, 171, 51));
        textEditTempResult = new QTextEdit(tab_2);
        textEditTempResult->setObjectName("textEditTempResult");
        textEditTempResult->setGeometry(QRect(60, 200, 431, 311));
        label = new QLabel(tab_2);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 20, 561, 16));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\321\202\320\265\321\200", nullptr));
        pushButtonLoad->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\273\320\276\320\263-\321\204\320\260\320\271\320\273", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\320\222\320\265\321\202\320\265\321\200", nullptr));
        pushButtonLoadTempLog->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 log-\321\204\320\260\320\271\320\273", nullptr));
        pushButtonCalculateTemp->setText(QCoreApplication::translate("MainWindow", "\320\262\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "A = 1,   B = 4000,   C = 100,   R1 = 32,   R2 = 32", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\320\242\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
