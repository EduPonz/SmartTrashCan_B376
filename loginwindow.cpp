#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "userdatabasemanager.h"

#include <QDebug>


LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::on_loginButton_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loginButton_clicked()
{
    QString userNameEntry = ui->userNameLineEdit->text();
    QString passWordEntry = ui->passWordLineEdit->text();

    UserDataBaseManager query;
    userName = query.DataBaseRetrieve(userNameEntry, passWordEntry);
    qWarning() << "LoginWindow::on_loginButton_clicked - ERROR: " << userName;

    if(!query.NOT_PERSON_FOUND.compare(userName))
    {
        ui->outPutLabel->setText(query.NOT_PERSON_FOUND);
    }else{
        ui->outPutLabel->setText("GG");
    }
}
