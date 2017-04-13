#ifndef EXTRAPICKUPWINDOW_H
#define EXTRAPICKUPWINDOW_H

#include <QWidget>
#include <QString>

namespace Ui {
class ExtraPickupWindow;
}

class ExtraPickupWindow : public QWidget
{
    Q_OBJECT

public:
    ExtraPickupWindow(QWidget *parent = 0, int id = 0);
    ~ExtraPickupWindow();

private slots:
    void on_extrapickupConfirmButton_clicked();

    void mailSent(QString);
private:

    QString EMAIL_USER_NAME;
    QString EMAIL_PASSWORD;
    QString EMAIL_SERVER;
    int EMAIL_PORT;
    QString EMAIL_SUBJECT;

    //Message to be sent variables

    //QString trashEmailMessage;

    bool trashSizeSmallBool;
    bool trashSizeMediumBool;
    bool trashSizeBigBool;

    //QString trashTextSmall;
    QString trashTextSmall;
    QString trashTextMedium;
    QString trashTextBig;

    QString trashQuantitySmall;
    QString trashQuantityMedium;
    QString trashQuantityBig;


    bool TRASH_TIME_ONE;
    bool TRASH_TIME_TWO;
    bool TRASH_TIME_FIVE;

    //QString trashQuantitySmall;
    int TRASH_QUANTITY_MEDIUM;
    int TRASH_QUANTITY_BIG;

    QString returnConstString(QString body);

    Ui::ExtraPickupWindow *ui;
};

#endif // EXTRAPICKUPWINDOW_H
