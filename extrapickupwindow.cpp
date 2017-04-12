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

    EMAIL_SUBJECT = "Smart Trash Can pick up order confirmed!";

    TRASH_SIZE_TEXT_MEDIUM = "medium";
    TRASH_SIZE_TEXT_BIG = "big";

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
    trashMediumSmallBool = ui->itemMediumCheckBox->checkState();
    trashSizeBigBool = ui->itemBigCheckBox->checkState();

    trashQuantitySmall = ui->spinBox->cleanText();
    trashQuantityMedium = ui->spinBox_2->cleanText();
    trashQuantityBig = ui->spinBox_3->cleanText();

    Smtp* smtp = new Smtp(EMAIL_USER_NAME, EMAIL_PASSWORD, EMAIL_SERVER, EMAIL_PORT);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    if(trashBigSmallBool == true)
        trashTextSmall = "small";
    if(trashBigMediumBool == true)
        trashTextMedium = "medium";
    if(trashBigBigBool == true)
        trashTextBig = "big";


    QString trashEmailMessage = "You have choosen " + trashTextSmall + " item(s) to be pick up with the quantity of "
            + trashQuantitySmall + ".\n";

    QString rcpt = "daviccs@gmail.com"; //get the email from the database
    QString commentMsg = trashEmailMessage + " Other comments;'" + ui->extrapickupCommentText->toPlainText() + "'";

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
