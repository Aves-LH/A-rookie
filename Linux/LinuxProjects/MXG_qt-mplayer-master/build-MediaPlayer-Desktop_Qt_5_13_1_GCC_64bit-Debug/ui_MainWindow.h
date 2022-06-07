/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonPause;
    QPushButton *pushButtonFastForward;
    QPushButton *pushButtonFastRewind;
    QPushButton *pushButtonNext;
    QPushButton *pushButtonBefore;
    QPushButton *pushButtonOpenFiles;
    QPushButton *pushButtonRemove;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(845, 538);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButtonPause = new QPushButton(centralWidget);
        pushButtonPause->setObjectName(QString::fromUtf8("pushButtonPause"));
        pushButtonPause->setGeometry(QRect(520, 380, 35, 30));
        pushButtonFastForward = new QPushButton(centralWidget);
        pushButtonFastForward->setObjectName(QString::fromUtf8("pushButtonFastForward"));
        pushButtonFastForward->setGeometry(QRect(560, 380, 40, 30));
        pushButtonFastRewind = new QPushButton(centralWidget);
        pushButtonFastRewind->setObjectName(QString::fromUtf8("pushButtonFastRewind"));
        pushButtonFastRewind->setGeometry(QRect(475, 380, 40, 30));
        pushButtonNext = new QPushButton(centralWidget);
        pushButtonNext->setObjectName(QString::fromUtf8("pushButtonNext"));
        pushButtonNext->setGeometry(QRect(605, 380, 40, 30));
        pushButtonBefore = new QPushButton(centralWidget);
        pushButtonBefore->setObjectName(QString::fromUtf8("pushButtonBefore"));
        pushButtonBefore->setGeometry(QRect(430, 380, 40, 30));
        pushButtonOpenFiles = new QPushButton(centralWidget);
        pushButtonOpenFiles->setObjectName(QString::fromUtf8("pushButtonOpenFiles"));
        pushButtonOpenFiles->setGeometry(QRect(770, 380, 30, 30));
        pushButtonRemove = new QPushButton(centralWidget);
        pushButtonRemove->setObjectName(QString::fromUtf8("pushButtonRemove"));
        pushButtonRemove->setGeometry(QRect(810, 380, 30, 30));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 845, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(pushButtonBefore, pushButtonFastRewind);
        QWidget::setTabOrder(pushButtonFastRewind, pushButtonPause);
        QWidget::setTabOrder(pushButtonPause, pushButtonFastForward);
        QWidget::setTabOrder(pushButtonFastForward, pushButtonNext);
        QWidget::setTabOrder(pushButtonNext, pushButtonOpenFiles);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButtonPause->setText(QCoreApplication::translate("MainWindow", "|  |", nullptr));
        pushButtonFastForward->setText(QCoreApplication::translate("MainWindow", ">>", nullptr));
        pushButtonFastRewind->setText(QCoreApplication::translate("MainWindow", "<<", nullptr));
        pushButtonNext->setText(QCoreApplication::translate("MainWindow", ">>|", nullptr));
        pushButtonBefore->setText(QCoreApplication::translate("MainWindow", "|<<", nullptr));
        pushButtonOpenFiles->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        pushButtonRemove->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
