#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userdatabasemanager.h"
#include <QDebug>


LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginSingupButton_clicked(){

    qDebug() << "LoginWindow::on_loginSingupButton_clicked - loginSingupButton CLICKED";
    userNameEntry = ui->userNameLineEdit->text();
    passWordEntry = ui->passWordLineEdit->text();

    loginQuery = loginDataBaseManager.userDataBaseRetrieve(userNameEntry, passWordEntry);

    if(!(loginQuery.first()))
    {
       bool insertSucced;
       insertSucced = loginDataBaseManager.userDatabaseInsert(userNameEntry, passWordEntry, ""
                                                , ""         , ""           , loginDataBaseManager.PAYMENT_METHOD_UNKNOWN
                                                , ""         , ""           , "");
        if (insertSucced){
            ui->outPutLabel->setText(loginDataBaseManager.USER_ADDED_SUCC);
            qDebug() << "LoginWindow::on_loginSingupButton_clicked - USER ADDED SUCC";
        }else{
            ui->outPutLabel->setText(loginDataBaseManager.USER_ADDED_FAIL);
            qDebug() << "LoginWindow::on_loginSingupButton_clicked - USER ADDED FAIL";
        }

    }else{
        ui->outPutLabel->setText(loginDataBaseManager.USER_EXISTS);
        qDebug() << "LoginWindow::on_loginSingupButton_clicked - USER EXISTS";
    }
}

void LoginWindow::on_loginButton_clicked()
{
    qDebug() << "LoginWindow::on_loginButton_clicked - loginButton CLICKED";
    userNameEntry = ui->userNameLineEdit->text();
    passWordEntry = ui->passWordLineEdit->text();

    loginQuery = loginDataBaseManager.userDataBaseRetrieve(userNameEntry, passWordEntry);

    if(loginQuery.first()){
        emit correctUser(userNameEntry);
        qDebug() << "LoginWindow::on_loginButton_clicked - correctUser signal sent";
    }else{
        ui->outPutLabel->setText(loginDataBaseManager.NOT_PERSON_FOUND);
    }
}

void LoginWindow::on_loginRemoveButton_clicked()
{
    loginDataBaseManager.userDatabaseDeleteAll();
}
