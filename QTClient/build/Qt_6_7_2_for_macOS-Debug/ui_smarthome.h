/********************************************************************************
** Form generated from reading UI file 'smarthome.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMARTHOME_H
#define UI_SMARTHOME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_smarthome
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTabWidget *tabWidget;
    QWidget *envTab;
    QLabel *lightLabel;
    QLabel *tempLabel;
    QLabel *humiLabel;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QPushButton *livingroomButton;
    QLabel *label_8;
    QPushButton *kitchenButton;
    QPushButton *washroomButton;
    QPushButton *bedroomButton;
    QPushButton *alarmButton;
    QWidget *timeTab;
    QWidget *alarmTab;
    QWidget *noteTab;

    void setupUi(QWidget *smarthome)
    {
        if (smarthome->objectName().isEmpty())
            smarthome->setObjectName("smarthome");
        smarthome->resize(551, 398);
        verticalLayout = new QVBoxLayout(smarthome);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(smarthome);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        tabWidget = new QTabWidget(smarthome);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMaximumSize(QSize(527, 16777215));
        QFont font;
        font.setPointSize(16);
        tabWidget->setFont(font);
        envTab = new QWidget();
        envTab->setObjectName("envTab");
        lightLabel = new QLabel(envTab);
        lightLabel->setObjectName("lightLabel");
        lightLabel->setGeometry(QRect(260, 10, 61, 31));
        tempLabel = new QLabel(envTab);
        tempLabel->setObjectName("tempLabel");
        tempLabel->setGeometry(QRect(190, 70, 61, 31));
        humiLabel = new QLabel(envTab);
        humiLabel->setObjectName("humiLabel");
        humiLabel->setGeometry(QRect(350, 70, 61, 31));
        label_2 = new QLabel(envTab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(160, 10, 111, 31));
        label_5 = new QLabel(envTab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(80, 70, 111, 31));
        label_6 = new QLabel(envTab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(250, 70, 111, 31));
        label_7 = new QLabel(envTab);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(110, 40, 261, 31));
        livingroomButton = new QPushButton(envTab);
        livingroomButton->setObjectName("livingroomButton");
        livingroomButton->setGeometry(QRect(190, 130, 101, 32));
        QFont font1;
        font1.setPointSize(15);
        livingroomButton->setFont(font1);
        label_8 = new QLabel(envTab);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(40, 150, 151, 31));
        kitchenButton = new QPushButton(envTab);
        kitchenButton->setObjectName("kitchenButton");
        kitchenButton->setGeometry(QRect(190, 170, 101, 32));
        kitchenButton->setFont(font1);
        washroomButton = new QPushButton(envTab);
        washroomButton->setObjectName("washroomButton");
        washroomButton->setGeometry(QRect(320, 170, 101, 32));
        washroomButton->setFont(font1);
        bedroomButton = new QPushButton(envTab);
        bedroomButton->setObjectName("bedroomButton");
        bedroomButton->setGeometry(QRect(320, 130, 101, 32));
        bedroomButton->setFont(font1);
        alarmButton = new QPushButton(envTab);
        alarmButton->setObjectName("alarmButton");
        alarmButton->setGeometry(QRect(120, 240, 231, 32));
        alarmButton->setFont(font1);
        tabWidget->addTab(envTab, QString());
        timeTab = new QWidget();
        timeTab->setObjectName("timeTab");
        tabWidget->addTab(timeTab, QString());
        alarmTab = new QWidget();
        alarmTab->setObjectName("alarmTab");
        tabWidget->addTab(alarmTab, QString());
        noteTab = new QWidget();
        noteTab->setObjectName("noteTab");
        tabWidget->addTab(noteTab, QString());

        verticalLayout->addWidget(tabWidget);


        retranslateUi(smarthome);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(smarthome);
    } // setupUi

    void retranslateUi(QWidget *smarthome)
    {
        smarthome->setWindowTitle(QCoreApplication::translate("smarthome", "Form", nullptr));
        label->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\"><span style=\" font-size:24pt;\">SmartHome Demo</span></p></body></html>", nullptr));
        lightLabel->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">0</p></body></html>", nullptr));
        tempLabel->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">0</p></body></html>", nullptr));
        humiLabel->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">0</p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">Illumination :</p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">Temperature :</p></body></html>", nullptr));
        label_6->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">Humidity :</p></body></html>", nullptr));
        label_7->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">-------------------------------</p></body></html>", nullptr));
        livingroomButton->setText(QCoreApplication::translate("smarthome", "Livingroom", nullptr));
        label_8->setText(QCoreApplication::translate("smarthome", "<html><head/><body><p align=\"center\">Lamp Control</p></body></html>", nullptr));
        kitchenButton->setText(QCoreApplication::translate("smarthome", "Kitchen", nullptr));
        washroomButton->setText(QCoreApplication::translate("smarthome", "Washroom", nullptr));
        bedroomButton->setText(QCoreApplication::translate("smarthome", "Bedroom", nullptr));
        alarmButton->setText(QCoreApplication::translate("smarthome", "Alarm", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(envTab), QCoreApplication::translate("smarthome", "Environment", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(timeTab), QCoreApplication::translate("smarthome", "Time", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(alarmTab), QCoreApplication::translate("smarthome", "Alarm Clock", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(noteTab), QCoreApplication::translate("smarthome", "Reminder", nullptr));
    } // retranslateUi

};

namespace Ui {
    class smarthome: public Ui_smarthome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMARTHOME_H
