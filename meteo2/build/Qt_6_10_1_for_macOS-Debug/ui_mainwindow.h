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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBoxInput;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButtonLoadTempLog;
    QPushButton *pushButtonLoad;
    QLabel *labelTemp;
    QLabel *labelWind;
    QLineEdit *lineEditWind;
    QLineEdit *lineEditTemp;
    QGroupBox *groupBoxParameters;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QDoubleSpinBox *doubleSpinBoxC;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxR1;
    QDoubleSpinBox *doubleSpinBoxR2;
    QLabel *label_5;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBoxB;
    QDoubleSpinBox *doubleSpinBoxA;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonCalculateTemp;
    QWidget *page_3;
    QLabel *label_7;
    QTextBrowser *textBrowser;
    QWidget *page_2;
    QLabel *label_6;
    QTableWidget *TableMtd;
    QTableWidget *TableMts;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1264, 744);
        MainWindow->setMaximumSize(QSize(16777215, 1000));
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(-20, -10, 1221, 691));
        page = new QWidget();
        page->setObjectName("page");
        verticalLayoutWidget = new QWidget(page);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(30, 30, 621, 451));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBoxInput = new QGroupBox(verticalLayoutWidget);
        groupBoxInput->setObjectName("groupBoxInput");
        groupBoxInput->setMaximumSize(QSize(16777215, 150));
        gridLayoutWidget = new QWidget(groupBoxInput);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(9, 30, 601, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonLoadTempLog = new QPushButton(gridLayoutWidget);
        pushButtonLoadTempLog->setObjectName("pushButtonLoadTempLog");

        gridLayout->addWidget(pushButtonLoadTempLog, 1, 2, 1, 1);

        pushButtonLoad = new QPushButton(gridLayoutWidget);
        pushButtonLoad->setObjectName("pushButtonLoad");

        gridLayout->addWidget(pushButtonLoad, 0, 2, 1, 1);

        labelTemp = new QLabel(gridLayoutWidget);
        labelTemp->setObjectName("labelTemp");

        gridLayout->addWidget(labelTemp, 1, 0, 1, 1);

        labelWind = new QLabel(gridLayoutWidget);
        labelWind->setObjectName("labelWind");

        gridLayout->addWidget(labelWind, 0, 0, 1, 1);

        lineEditWind = new QLineEdit(gridLayoutWidget);
        lineEditWind->setObjectName("lineEditWind");

        gridLayout->addWidget(lineEditWind, 0, 1, 1, 1);

        lineEditTemp = new QLineEdit(gridLayoutWidget);
        lineEditTemp->setObjectName("lineEditTemp");

        gridLayout->addWidget(lineEditTemp, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBoxInput);

        groupBoxParameters = new QGroupBox(verticalLayoutWidget);
        groupBoxParameters->setObjectName("groupBoxParameters");
        gridLayoutWidget_2 = new QWidget(groupBoxParameters);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(9, 20, 601, 188));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        doubleSpinBoxC = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxC->setObjectName("doubleSpinBoxC");
        doubleSpinBoxC->setMaximum(100000.000000000000000);

        gridLayout_2->addWidget(doubleSpinBoxC, 2, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        doubleSpinBoxR1 = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxR1->setObjectName("doubleSpinBoxR1");
        doubleSpinBoxR1->setMaximum(100000.000000000000000);

        gridLayout_2->addWidget(doubleSpinBoxR1, 3, 1, 1, 1);

        doubleSpinBoxR2 = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxR2->setObjectName("doubleSpinBoxR2");
        doubleSpinBoxR2->setMaximum(100000.000000000000000);

        gridLayout_2->addWidget(doubleSpinBoxR2, 4, 1, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName("label_5");

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        label = new QLabel(gridLayoutWidget_2);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_2);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        doubleSpinBoxB = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxB->setObjectName("doubleSpinBoxB");
        doubleSpinBoxB->setMaximum(100000.000000000000000);

        gridLayout_2->addWidget(doubleSpinBoxB, 1, 1, 1, 1);

        doubleSpinBoxA = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxA->setObjectName("doubleSpinBoxA");
        doubleSpinBoxA->setMaximum(100000.000000000000000);

        gridLayout_2->addWidget(doubleSpinBoxA, 0, 1, 1, 1);

        horizontalLayoutWidget = new QWidget(groupBoxParameters);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(10, 210, 601, 71));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonCalculateTemp = new QPushButton(horizontalLayoutWidget);
        pushButtonCalculateTemp->setObjectName("pushButtonCalculateTemp");
        pushButtonCalculateTemp->setMinimumSize(QSize(0, 50));
        pushButtonCalculateTemp->setMaximumSize(QSize(300, 16777215));

        horizontalLayout->addWidget(pushButtonCalculateTemp);


        verticalLayout->addWidget(groupBoxParameters);

        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        label_7 = new QLabel(page_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(330, 90, 111, 16));
        textBrowser = new QTextBrowser(page_3);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setGeometry(QRect(60, 120, 331, 371));
        stackedWidget->addWidget(page_3);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        label_6 = new QLabel(page_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(550, 10, 161, 16));
        TableMtd = new QTableWidget(page_2);
        TableMtd->setObjectName("TableMtd");
        TableMtd->setGeometry(QRect(40, 50, 541, 621));
        TableMts = new QTableWidget(page_2);
        TableMts->setObjectName("TableMts");
        TableMts->setGeometry(QRect(600, 50, 551, 621));
        stackedWidget->addWidget(page_2);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        action->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\321\202\320\265\321\200", nullptr));
        groupBoxInput->setTitle(QCoreApplication::translate("MainWindow", "\320\237\321\200\320\270\320\272\321\200\320\265\320\277\320\270\321\202\320\265 log-\321\204\320\260\320\271\320\273\321\213 \320\274\320\265\321\202\320\265\320\276\320\272\320\276\320\274\320\277\320\273\320\265\320\272\321\201\320\260:", nullptr));
        pushButtonLoadTempLog->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\204\320\260\320\271\320\273", nullptr));
        pushButtonLoad->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\204\320\260\320\271\320\273 ", nullptr));
        labelTemp->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\270 \321\202\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\321\213:", nullptr));
        labelWind->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\270 \320\262\320\265\321\202\321\200\320\260:", nullptr));
        groupBoxParameters->setTitle(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\275\320\260\321\207\320\260\320\273\321\214\320\275\321\213\320\265 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 R1", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 R2", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 \320\220", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 \320\222", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 C", nullptr));
        pushButtonCalculateTemp->setText(QCoreApplication::translate("MainWindow", "\320\262\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202\321\213", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\320\260", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
