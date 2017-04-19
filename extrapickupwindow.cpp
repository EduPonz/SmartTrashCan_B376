#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"
#include "smtp.h"
#include "userdatabasemanager.h"

#include <QDebug>
#include <QSqlError>
#include <string>
#include <string.h>
#include <historydatabasemanager.h>

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

void ExtraPickupWindow::pickupTimeHandler(){

    trashTimeOne = ui->time1CheckBox->checkState();
    trashTimeTwo = ui->time2CheckBox->checkState();
    trashTimeFive = ui->time5CheckBox->checkState();

    if (trashTimeOne == true){
        ui->timeLabel->setText("Price: 15 Bottle caps");
        timePrice = 15;
        trashTime = "Today";
        intTrashTime = 1;
    }
    if (trashTimeTwo == true){
        ui->timeLabel->setText("Price: 10 Bottle caps");
        timePrice = 10;
        trashTime = "2 days";
        intTrashTime = 2;
    }
    if (trashTimeFive == true){
        ui->timeLabel->setText("Price: 5 Bottle caps");
        timePrice = 5;
        trashTime = "5 days";
        intTrashTime = 5;
    }

}

void ExtraPickupWindow::sizeTimeMoneyHandler(){

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

    price = ui->spinBox->cleanText().toInt() * 10 + ui->spinBox_2->cleanText().toInt() * 20 + ui->spinBox_3->cleanText().toInt() * 30;
    if (trashSizeSmallBool == true || trashSizeMediumBool == true || trashSizeBigBool == true) {
        request = request + ". The price of the selected (items) is; " + QString::number(price) + " Bottle caps. \n";
        request = request + "The time of pick up is in; " + trashTime + ". For the price of " + QString::number(timePrice)
                + ".\n"
                + "The total price of your services is; "
                + QString::number(timePrice + price)
                + ". ";
    }
    ui->totalPriceLabel->setText(QString::number(timePrice + price) + " Bottle caps.");
    ui->priceLabel->setText("Price of items: " + QString::number(price) + " Bottle caps.");
}

void ExtraPickupWindow::on_extrapickupConfirmButton_clicked()
{
    Smtp* smtp = new Smtp(EMAIL_USER_NAME, EMAIL_PASSWORD, EMAIL_SERVER, EMAIL_PORT);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    pickupTimeHandler();
    sizeTimeMoneyHandler();

    QString commentMsg = request + "You have added the following comments; '"
            + ui->extrapickupCommentText->toPlainText() + "'";
    QString emailAddress = dataBase.userDataBaseRetrieveUserEmail(userId);
    historyManager.historyDatabaseInsert(userId, trashSizeSmallBool, trashSizeMediumBool, trashSizeBigBool,
                                         ui->spinBox->cleanText().toInt(), ui->spinBox_2->cleanText().toInt(),
                                         ui->spinBox_3->cleanText().toInt(), ui->extrapickupCommentText->toPlainText(),
                                         intTrashTime, price, timePrice, (timePrice+price));
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
