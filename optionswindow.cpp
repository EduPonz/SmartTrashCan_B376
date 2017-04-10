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

void OptionsWindow::on_optionsApplyChangesButton_clicked()
{
    QString newUserName = ui->optionsUsernameLineEdit->text();
    QString password = ui->optionsPasswordLineEdit->text();
    QString full_name = ui->optionsFullNameLineEdit->text();
    QString address =  ui->optionsAddressLineEdit->text();
    QString phone_number = ui->optionsPhoneNumberLineEdit->text();
    QString payment_method;
    QString card_number = ui->optionsCardNumberLineEdit->text();
    QString cvv = ui->optionsCvvLineEdit->text();
    QString expiry_date = ui->optionsExpirationDateLineEdit->text();

    if (ui->optionsVisaCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_VISA;
    }else if (ui->optionsMastercardCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_MASTERCARD;
    }else if (ui->optionsBottlecapsCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_BOTTLECAPS;
    }else {
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_UNKNOWN;
    }
    optionsDatabaseManager.userDataBaseUpdate(userName, newUserName, password,
                                              full_name, address, phone_number,
                                              payment_method, card_number, cvv, expiry_date);

    if ((!newUserName.isEmpty()) && (!password.isEmpty()) && (!full_name.isEmpty()) && (!address.isEmpty())){
        if (!optionsDatabaseManager.availableUserName(newUserName) && !newUserName.compare(userName)){
            emit apply_changes();
            ui->optionsOutPutLabel->setText("Your user has been updated");
        }else{
            ui->optionsOutPutLabel->setText("Username not avilable. Try another one");
        }
    }else{
            ui->optionsOutPutLabel->setText("Some fields are missing");
    }
}
