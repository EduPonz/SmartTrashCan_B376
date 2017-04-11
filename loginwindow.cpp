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

void LoginWindow::on_loginSingupButton_clicked()
{
    emit signUpSignal();
}

void LoginWindow::on_loginButton_clicked()
{
    userNameEntry = ui->userNameLineEdit->text();
    passWordEntry = ui->passWordLineEdit->text();

    loginQuery = loginDataBaseManager.userDataBaseRetrieve(userNameEntry, passWordEntry);
    qDebug() << loginQuery.executedQuery();

    if(loginQuery.first()){
        QString userIdString = loginQuery.value(0).toString();
        int id = userIdString.toInt();
        userIdString.number(id);
        qDebug() << "LoginWindow::on_loginButton_clicked - ID: " + userIdString;
        emit correctUser(id);
    }else{
        ui->outPutLabel->setText(loginDataBaseManager.NOT_PERSON_FOUND);
    }
}

void LoginWindow::on_loginRemoveButton_clicked()
{
    loginDataBaseManager.userDatabaseDeleteAll();
}
