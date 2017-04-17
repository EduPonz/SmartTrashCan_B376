#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"
#include "smtp.h"
#include "userdatabasemanager.h"

#include <QDebug>
#include <QSqlError>

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent, int id) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);

    userId = id;
    qDebug() << "ExtraPickupWindow::extraPickUpSlot: " << id;

    EMAIL_USER_NAME = "stcb376";
    EMAIL_PASSWORD = "SimplePassword";
    EMAIL_SERVER = "smtp.gmail.com";
    EMAIL_PORT = 465;

    EMAIL_SUBJECT = "Smart Trash Can pick up order confirmed!";
}

void ExtraPickupWindow::on_extrapickupConfirmButton_clicked()
{
    trashTextSmall = "";
    trashTextMedium = "";
    trashTextBig = "";

    trashQuantitySmall = "";
    trashQuantityMedium = "";
    trashQuantityBig = "";

    trashSizeSmallBool = ui->itemSmallCheckBox->checkState();
    trashSizeMediumBool = ui->itemMediumCheckBox->checkState();
    trashSizeBigBool = ui->itemBigCheckBox->checkState();

    Smtp* smtp = new Smtp(EMAIL_USER_NAME, EMAIL_PASSWORD, EMAIL_SERVER, EMAIL_PORT);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    if(trashSizeSmallBool == true){
        trashTextSmall = "small size;";
        trashQuantitySmall = ui->spinBox->cleanText();
    }

    if(trashSizeMediumBool == true){
        trashTextMedium = "medium size;";
        trashQuantityMedium = ui->spinBox_2->cleanText();
    }
    if(trashSizeBigBool == true){
        trashTextBig = "large size;";
        trashQuantityBig = ui->spinBox_3->cleanText();
    }


    QString trashEmailMessage = "You have requested; "
            + trashQuantitySmall  + " "  + trashTextSmall + " "
            + trashQuantityMedium + " " + trashTextMedium + " "
            + trashQuantityBig    + " " + trashTextBig
            + " item(s) to be picked up " + ".\n";

    QString rcpt = "daviccs@gmail.com"; //get the email from the database
    qDebug() << "ExtraPickupWindow::on_extrapickupConfirmButton_clicked: " << dataBase.emailAddress;
    QString commentMsg = trashEmailMessage + "You have added the following comments; '"
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
