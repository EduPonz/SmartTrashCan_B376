/*! \class HistoryDatabaseManager class.
    \brief This class handles the historyData table in the SQlite database.
    It stores the order history iformation of each user and displays it into the
    order history table in the application, under OptionsWindow.
*/

#ifndef HISTORYDATABASEMANAGER_H
#define HISTORYDATABASEMANAGER_H

#include <QSqlDatabase>
#include "userdatabasemanager.h"
#include "historydatabasemanager.h"

#include <QStringList>

class HistoryDatabaseManager
{
public:
    HistoryDatabaseManager();

    QString COLUMN_ID;
    QString COLUMN_USERID;

    QString COLUMN_SELECTED_ITEM_SMALL;
    QString COLUMN_SELECTED_ITEM_MEDIUM;
    QString COLUMN_SELECTED_ITEM_BIG;

    QString COLUMN_ITEM_SMALL_QUANTITY; //1-9
    QString COLUMN_ITEM_MEDIUM_QUANTITY; //1-9
    QString COLUMN_ITEM_BIG_QUANTITY; //1-9

    QString COLUMN_COMMENTS;
    QString COLUMN_PICKUP_TIME; //1-3
    QString COLUMN_ITEM_PRICE;
    QString COLUMN_TIME_PRICE;
    QString COLUMN_TOTAL_PRICE;

    QString COLUMN_PICK_UP_DATE;
    QString COLUMN_INVOICE;

    int rows;

    QStringList small_item_qty;
    int medium_item_qty;

    bool historyDatabaseInsert(int userID, bool selected_small, bool selected_medium, bool selected_big,
                               int small_qty, int medium_qty, int big_qty, QString comms, int pickup_time,
                               int item_price, int time_price, int total_price, QString time, int invoice);   /*!<Populates the table with the purchase history information to the corresponding user ID */
    QSqlQuery rowNumberRetrieve(int userID);  /*!<Retrieves the number of rows with the same user ID. */
    QSqlQuery historyDatabaseRetrieve(int userID); /*!<Retrieves the information from the historyData table to be insterted into the order history table. */

private:
    QString TABLE_NAME;

    UserDataBaseManager userDatabaseManager;

   // void trashInfoDatabaseConnect();
    void historyDatabaseInit(); /*!<Creates the historyData table in the database with the corresponding colunmns.   */
    bool isExist(); /*!<Check whether the databse is open and can be edited. Returns false if it is closed. */
    bool historyDatabaseDelete(int userID); /*!< Deletes the user's account and corresponding ID row from the historyData table *if the user chooses to terminate their account.*/
};

#endif // HISTORYDATABASEMANAGER_H
