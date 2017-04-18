#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"
#include "smtp.h"
#include "userdatabasemanager.h"

#include <QDebug>
#include <QSqlError>
#include <string>
#include <string.h>

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent, int id) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);

    userId = id;


    EMAIL_USER_NAME = "stcb376";
    EMAIL_PASSWORD = "SimplePassword";
    EMAIL_SERVER = "smtp.gmail.com";
    EMAIL_PORT = 465;

    EMAIL_SUBJECT = "Smart Trash Can pick up order confirmed!";
}

void ExtraPickupWindow::sizeTimeMoneyHandler(){


    //QString moneyHandler[3][3];
    trashSizeSmallBool = ui->itemSmallCheckBox->checkState();
    trashSizeMediumBool = ui->itemMediumCheckBox->checkState();
    trashSizeBigBool = ui->itemBigCheckBox->checkState();
    request = "";
    if (trashSizeSmallBool == true || trashSizeMediumBool == true || trashSizeBigBool == true) {
        request = "You have selected ";
    }


    if(trashSizeSmallBool == true){
        request = request + "small size item(s) with the quantity of " + ui->spinBox->cleanText() + " for 10 Bottle caps per item";
    }

    if(trashSizeMediumBool == true){
        request = request + ", medium size item(s) with the quantity of " + ui->spinBox_2->cleanText() + " for 20 Bottle caps per item";
    }

    if(trashSizeBigBool == true){
        request = request + ", and large size item(s) with the quantity of " + ui->spinBox_3->cleanText() + " for 30 Bottle caps per item";
    }

    int price = ui->spinBox->cleanText().toInt() * 10 + ui->spinBox_2->cleanText().toInt() * 20 + ui->spinBox_3->cleanText().toInt() * 30;
    if (trashSizeSmallBool == true || trashSizeMediumBool == true || trashSizeBigBool == true) {
        request = request + ". The price of the service is; " + QString::number(price) + " Bottle caps. \n";
    }
}

void ExtraPickupWindow::on_extrapickupConfirmButton_clicked()
{
    Smtp* smtp = new Smtp(EMAIL_USER_NAME, EMAIL_PASSWORD, EMAIL_SERVER, EMAIL_PORT);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    sizeTimeMoneyHandler();

    QString commentMsg = request + "You have added the following comments; '"
            + ui->extrapickupCommentText->toPlainText() + "'";
    QString emailAddress = dataBase.userDataBaseRetrieveUserEmail(userId);

    smtp->sendMail(EMAIL_USER_NAME, emailAddress, EMAIL_SUBJECT, commentMsg);
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
