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
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QWidget *centralwidget;
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
    QPushButton *pushButtonCalculateTemp;
    QGroupBox *pushButtonCalculate;
    QTextEdit *textEditTempResult;
    QGroupBox *groupBoxResults;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(753, 792);
        action = new QAction(MainWindow);
        action->setObjectName("action");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(20, 20, 691, 801));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBoxInput = new QGroupBox(verticalLayoutWidget);
        groupBoxInput->setObjectName("groupBoxInput");
        groupBoxInput->setMaximumSize(QSize(16777215, 150));
        gridLayoutWidget = new QWidget(groupBoxInput);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(9, 30, 661, 80));
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
        gridLayoutWidget_2->setGeometry(QRect(9, 20, 661, 188));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        doubleSpinBoxC = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxC->setObjectName("doubleSpinBoxC");

        gridLayout_2->addWidget(doubleSpinBoxC, 2, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        doubleSpinBoxR1 = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxR1->setObjectName("doubleSpinBoxR1");

        gridLayout_2->addWidget(doubleSpinBoxR1, 3, 1, 1, 1);

        doubleSpinBoxR2 = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxR2->setObjectName("doubleSpinBoxR2");

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

        gridLayout_2->addWidget(doubleSpinBoxB, 1, 1, 1, 1);

        doubleSpinBoxA = new QDoubleSpinBox(gridLayoutWidget_2);
        doubleSpinBoxA->setObjectName("doubleSpinBoxA");

        gridLayout_2->addWidget(doubleSpinBoxA, 0, 1, 1, 1);

        pushButtonCalculateTemp = new QPushButton(gridLayoutWidget_2);
        pushButtonCalculateTemp->setObjectName("pushButtonCalculateTemp");

        gridLayout_2->addWidget(pushButtonCalculateTemp, 5, 0, 1, 1);


        verticalLayout->addWidget(groupBoxParameters);

        pushButtonCalculate = new QGroupBox(verticalLayoutWidget);
        pushButtonCalculate->setObjectName("pushButtonCalculate");
        textEditTempResult = new QTextEdit(pushButtonCalculate);
        textEditTempResult->setObjectName("textEditTempResult");
        textEditTempResult->setGeometry(QRect(13, 33, 331, 171));

        verticalLayout->addWidget(pushButtonCalculate);

        groupBoxResults = new QGroupBox(verticalLayoutWidget);
        groupBoxResults->setObjectName("groupBoxResults");

        verticalLayout->addWidget(groupBoxResults);

        pushButtonCalculate->raise();
        groupBoxParameters->raise();
        groupBoxInput->raise();
        groupBoxResults->raise();
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
        groupBoxInput->setTitle(QCoreApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265", nullptr));
        pushButtonLoadTempLog->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\204\320\260\320\271\320\273", nullptr));
        pushButtonLoad->setText(QCoreApplication::translate("MainWindow", "\320\267\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \321\204\320\260\320\271\320\273 ", nullptr));
        labelTemp->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\270 \321\202\320\265\320\274\320\277\320\265\321\200\320\260\321\202\321\203\321\200\321\213:", nullptr));
        labelWind->setText(QCoreApplication::translate("MainWindow", "\320\233\320\276\320\263\320\270 \320\262\320\265\321\202\321\200\320\260:", nullptr));
        groupBoxParameters->setTitle(QCoreApplication::translate("MainWindow", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213 \321\200\320\260\321\201\321\207\320\265\321\202\320\260", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 R1", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 R2", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 \320\220", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 \320\222", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", " \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200 C", nullptr));
        pushButtonCalculateTemp->setText(QCoreApplication::translate("MainWindow", "\320\262\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202", nullptr));
        pushButtonCalculate->setTitle(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\260\321\202\321\214 \321\200\320\260\321\201\321\207\320\265\321\202", nullptr));
        groupBoxResults->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
