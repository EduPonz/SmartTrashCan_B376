#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"
#include "smtp.h"

#include <QDebug>

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent, int id) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);

    EMAIL_USER_NAME = "stcb376";
    EMAIL_PASSWORD = "SimplePassword";
    EMAIL_SERVER = "smtp.gmail.com";
    EMAIL_PORT = 465;

    EMAIL_SUBJECT = "Smart Trash can pick up order confirmed!";
}

void ExtraPickupWindow::on_extrapickupConfirmButton_clicked()
{
    Smtp* smtp = new Smtp(EMAIL_USER_NAME, EMAIL_PASSWORD, EMAIL_SERVER, EMAIL_PORT);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    QString rcpt = "e.ponzs@gmail.com";    
    QString commentMsg = ui->extrapickupCommentText->toPlainText();

    smtp->sendMail(EMAIL_USER_NAME, rcpt, EMAIL_SUBJECT, commentMsg);
}

void ExtraPickupWindow::mailSent(QString status)
{
    if(status == "Message sent"){
        ui->extraPickUpUOutputLabel->setText("Your message has been sent!");
    }
}

ExtraPickupWindow::~ExtraPickupWindow()
{
    delete ui;
}
