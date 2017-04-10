#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "userdatabasemanager.h"
#include <QDebug>
#include <QString>

SignUpWindow::SignUpWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SignUpWindow)
{
    ui->setupUi(this);
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

void SignUpWindow::on_signUpSingUpButton_clicked()
{
    userName = ui->signUpUsernameLineEdit->text();
    password = ui->signUpPasswordLineEdit->text();
    full_name = ui->signUpFullNameLineEdit->text();
    address = ui->signUpAddressLineEdit->text();
    phone_number = ui->signUpPhoneNumberLineEdit->text();
    card_number = ui->signUpCardNumberLineEdit->text();
    cvv = ui->signUpCvvLineEdit->text();
    expiration_date = ui->signUpExpirationDateLineEdit->text();
    if (ui->signUpVisaCheckBox->isChecked()){
        payment_method = signUpDatabaseManager.PAYMENT_METHOD_VISA;
    }else if (ui->signUpMastercardCheckBox->isChecked()){
        payment_method = signUpDatabaseManager.PAYMENT_METHOD_MASTERCARD;
    }else if (ui->signUpBottlecapsCheckBox->isChecked()){
        payment_method = signUpDatabaseManager.PAYMENT_METHOD_BOTTLECAPS;
    }else{
        payment_method = signUpDatabaseManager.PAYMENT_METHOD_UNKNOWN;
    }
    insertNewUser();
}

void SignUpWindow::insertNewUser()
{
    signUpQuery = signUpDatabaseManager.userDataBaseRetrieve(userName, password);

    if(!(signUpQuery.first()))
    {
       bool insertSucced;
       insertSucced = signUpDatabaseManager.userDatabaseInsert(userName, password, full_name,
                                                                address, phone_number, payment_method,
                                                                card_number, cvv, expiration_date);
        if (insertSucced){
            emit signUpAddUserSignal(userName);
            qDebug() << "SignUpWindow::insertNewUser - USER ADDED SUCC";
        }else{
            ui->signUpOutPutLabel->setText(signUpDatabaseManager.USER_ADDED_FAIL);
            qDebug() << "SignUpWindow::insertNewUser - USER ADDED FAIL";
        }

    }else{
        ui->signUpOutPutLabel->setText(signUpDatabaseManager.USER_EXISTS);
        qDebug() << "SignUpWindow::on_loginSingupButton_clicked - USER EXISTS";
    }
}


void SignUpWindow::on_signUpCancelButton_clicked()
{
    emit singUpCancelSignUp();
}
