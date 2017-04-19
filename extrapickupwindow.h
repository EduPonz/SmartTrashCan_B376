#ifndef EXTRAPICKUPWINDOW_H
#define EXTRAPICKUPWINDOW_H

#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include "userdatabasemanager.h"
#include "historydatabasemanager.h"

namespace Ui {
class ExtraPickupWindow;
}

class ExtraPickupWindow : public QWidget
{
    Q_OBJECT

public:
    ExtraPickupWindow(QWidget *parent = 0, int id = 0);

    QSqlDatabase userDataBase;
    int userId;

    ~ExtraPickupWindow();

private slots:
    void on_extrapickupConfirmButton_clicked();

    void mailSent(QString);
private:

    UserDataBaseManager dataBase;
    HistoryDatabaseManager historyManager;

    void sizeTimeMoneyHandler();
    void pickupTimeHandler();
    void saveHistoryData(QString status);
    int timePrice;
    int intTrashTime;
    int price;
    QString trashTime;

    QString trashEmailMessage;

    QString EMAIL_USER_NAME;
    QString EMAIL_PASSWORD;
    QString EMAIL_SERVER;
    int EMAIL_PORT;
    QString EMAIL_SUBJECT;

    bool trashSizeSmallBool;
    bool trashSizeMediumBool;
    bool trashSizeBigBool;

    QString request;

    bool trashTimeOne;
    bool trashTimeTwo;
    bool trashTimeFive;

    Ui::ExtraPickupWindow *ui;
};

#endif // EXTRAPICKUPWINDOW_H
