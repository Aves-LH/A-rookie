/********************************************************************************
** Form generated from reading UI file 'mediaplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPLAYER_H
#define UI_MEDIAPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customslider.h>

QT_BEGIN_NAMESPACE

class Ui_MediaPlayer
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    CustomSlider *horizontalSlider;
    QPushButton *pushButton_Volume;
    QPushButton *pushButton_Player;
    QPushButton *pushButton_Open;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MediaPlayer)
    {
        if (MediaPlayer->objectName().isEmpty())
            MediaPlayer->setObjectName(QString::fromUtf8("MediaPlayer"));
        MediaPlayer->resize(591, 507);
        centralWidget = new QWidget(MediaPlayer);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_3 = new QVBoxLayout(centralWidget);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        verticalLayout_2->addLayout(verticalLayout);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSlider = new CustomSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        pushButton_Volume = new QPushButton(centralWidget);
        pushButton_Volume->setObjectName(QString::fromUtf8("pushButton_Volume"));

        horizontalLayout->addWidget(pushButton_Volume);

        pushButton_Player = new QPushButton(centralWidget);
        pushButton_Player->setObjectName(QString::fromUtf8("pushButton_Player"));

        horizontalLayout->addWidget(pushButton_Player);

        pushButton_Open = new QPushButton(centralWidget);
        pushButton_Open->setObjectName(QString::fromUtf8("pushButton_Open"));

        horizontalLayout->addWidget(pushButton_Open);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        MediaPlayer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MediaPlayer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 591, 22));
        MediaPlayer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MediaPlayer);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MediaPlayer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MediaPlayer);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MediaPlayer->setStatusBar(statusBar);

        retranslateUi(MediaPlayer);

        QMetaObject::connectSlotsByName(MediaPlayer);
    } // setupUi

    void retranslateUi(QMainWindow *MediaPlayer)
    {
        MediaPlayer->setWindowTitle(QCoreApplication::translate("MediaPlayer", "MediaPlayer", nullptr));
        label->setText(QCoreApplication::translate("MediaPlayer", "\346\225\231\345\255\246\350\247\206\351\242\221\346\222\255\346\224\276\345\214\272", nullptr));
        pushButton_Volume->setText(QCoreApplication::translate("MediaPlayer", "\351\237\263\351\207\217", nullptr));
        pushButton_Player->setText(QCoreApplication::translate("MediaPlayer", "\346\222\255\346\224\276", nullptr));
        pushButton_Open->setText(QCoreApplication::translate("MediaPlayer", "\346\211\223\345\274\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MediaPlayer: public Ui_MediaPlayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H
