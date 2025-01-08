/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *idEdit;
    QLineEdit *pwdEdit;
    QPushButton *regButton;
    QPushButton *loginButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(443, 401);
        label = new QLabel(Widget);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 30, 271, 91));
        label_2 = new QLabel(Widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 150, 221, 31));
        label_3 = new QLabel(Widget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 210, 221, 31));
        idEdit = new QLineEdit(Widget);
        idEdit->setObjectName("idEdit");
        idEdit->setGeometry(QRect(190, 150, 141, 31));
        pwdEdit = new QLineEdit(Widget);
        pwdEdit->setObjectName("pwdEdit");
        pwdEdit->setGeometry(QRect(190, 210, 141, 31));
        regButton = new QPushButton(Widget);
        regButton->setObjectName("regButton");
        regButton->setGeometry(QRect(60, 280, 100, 51));
        QFont font;
        font.setPointSize(15);
        regButton->setFont(font);
        loginButton = new QPushButton(Widget);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(290, 280, 100, 51));
        loginButton->setFont(font);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" font-size:24pt;\">SmartHome Demo Login</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt;\">Device ID</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\"><span style=\" font-size:18pt;\">Password</span></p></body></html>", nullptr));
        regButton->setText(QCoreApplication::translate("Widget", "Register", nullptr));
        loginButton->setText(QCoreApplication::translate("Widget", "Log in", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
