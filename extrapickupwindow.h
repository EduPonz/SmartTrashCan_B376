/*! \class ExtraPickupWindow class.
    \brief Handles the extra pick up services UI and its backend logic.
    It allows the user to choose items to be picked up, specify a date for pick up
    and provide additional information regarding the items.
*/

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
    ExtraPickupWindow(QWidget *parent = 0, int id = 0); /*!<Assigns the UI to be a extrapickupwindow.ui. Initializes the required variables.*/

    QSqlDatabase userDataBase;
    int userId;

    ~ExtraPickupWindow(); /*!<Deletes the extrapickupwindow.ui UI.*/

signals:
    void cancel_service();

private slots:
    void on_extrapickupConfirmButton_clicked(); /*!<Gathers the data for the order confirmation email. Sends the gathered information to the HistoryDatabaseManager class for storage.*/

    void mailSent(QString); /*!<Checks if the email is successfully sent. If so, resets all the checkboxes and spinboxes to default position.*/
    void on_pushButton_2_clicked(); /*!<Cancels the service.*/

private:

    UserDataBaseManager dataBase;
    HistoryDatabaseManager historyManager;

    bool sizeTimeMoneyHandler(); /*!<Calcules the total price of the purchase from the number of items, the size and the date choosen. Also calculates the pick up date based on the time choosen*/
    void pickupTimeHandler(); /*!<Reads the checkboxes for the choosen time and displays the price of the time.*/
    int timePrice;
    int intTrashTime;
    int price;
    int invoiceNumber;
    QString currentDate;
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
