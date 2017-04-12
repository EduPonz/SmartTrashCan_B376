#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"
#include "smtp.h"

#include <QDebug>

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent, int id) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);
}

ExtraPickupWindow::~ExtraPickupWindow()
{
    delete ui;
}

void ExtraPickupWindow::on_extrapickupConfirmButton_clicked()
{
    QString username = "stcb376";
    QString password = "SimplePassword";
    QString server = "smtp.gmail.com";
    QString port = "465";
    Smtp* smtp = new Smtp(username, password, server, port.toInt());
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    QString rcpt = "e.ponzs@gmail.com";
    QString subject = "another test";
    QString msg = "testerino";

    smtp->sendMail(username, rcpt, subject, msg);
}

void ExtraPickupWindow::mailSent(QString status)
{
    if(status == "Message sent"){
        ui->extraPickUpUOutputLabel->setText("Your message has been sent!");
    }
}
