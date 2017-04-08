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

    databaseUserName = query.userDataBaseRetrieve(userNameEntry, passWordEntry);

    if(!query.NOT_PERSON_FOUND.compare(databaseUserName))
    {
        query.userDatabaseInsert(userNameEntry, passWordEntry);
        ui->outPutLabel->setText(query.USER_ADDED_SUCC);
        qDebug() << "LoginWindow::on_loginSingupButton_clicked - USER ADDED SUCC";
    }else{
        ui->outPutLabel->setText(query.USER_EXISTS);
        qDebug() << "LoginWindow::on_loginSingupButton_clicked - USER EXISTS";
    }
}

void LoginWindow::on_loginButton_clicked()
{
    qDebug() << "LoginWindow::on_loginButton_clicked - loginButton CLICKED";
    userNameEntry = ui->userNameLineEdit->text();
    passWordEntry = ui->passWordLineEdit->text();

    databaseUserName = query.userDataBaseRetrieve(userNameEntry, passWordEntry);

    if(!query.NOT_PERSON_FOUND.compare(databaseUserName))
    {
        ui->outPutLabel->setText(query.NOT_PERSON_FOUND);
    }else{
        emit correctUser(userNameEntry);
        qDebug() << "LoginWindow::on_loginButton_clicked - correctUser signal sent";
    }
}
