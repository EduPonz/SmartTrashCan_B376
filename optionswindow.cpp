#include "optionswindow.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "userdatabasemanager.h"
#include "ui_optionswindow.h"
#include <QDebug>
#include <QSqlQuery>

OptionsWindow::OptionsWindow(QWidget *parent, QString user) : QWidget(parent), ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    userName = user;
    populate();
}

void OptionsWindow::populate()
{
    optionsQuery = optionsDatabaseManager.userDataBaseRetrieve(userName);
    if (optionsQuery.isActive()){
        optionsQuery.first();
        ui->optionsUsernameLineEdit->setText(optionsQuery.value(0).toString());
        ui->optionsPasswordLineEdit->setText(optionsQuery.value(1).toString());
        ui->optionsFullNameLineEdit->setText(optionsQuery.value(2).toString());
        ui->optionsAddressLineEdit->setText(optionsQuery.value(3).toString());
        ui->optionsPhoneNumberLineEdit->setText(optionsQuery.value(4).toString());
        ui->optionsCardNumberLineEdit->setText(optionsQuery.value(6).toString());
        ui->optionsCvvLineEdit->setText(optionsQuery.value(7).toString());
        ui->optionsExpirationDateLineEdit->setText(optionsQuery.value(8).toString());
        if (!optionsQuery.value(5).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_VISA)){
            ui->optionsVisaCheckBox->setChecked(1);
        }else if (!optionsQuery.value(5).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_MASTERCARD)){
            ui->optionsMastercardCheckBox->setChecked(1);
        }else if (!optionsQuery.value(5).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_BOTTLECAPS)){
            ui->optionsBottlecapsCheckBox->setChecked(1);
        }else{
            ui->optionsVisaCheckBox->setChecked(0);
            ui->optionsMastercardCheckBox->setChecked(0);
            ui->optionsBottlecapsCheckBox->setChecked(0);
        }
    }
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}
