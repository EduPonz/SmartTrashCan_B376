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
                               int item_price, int time_price, int total_price, QString time, int invoice);
    QSqlQuery rowNumberRetrieve(int userID);
    QSqlQuery historyDatabaseRetrieve(int userID);

private:
    QString TABLE_NAME;

    UserDataBaseManager userDatabaseManager;

   // void trashInfoDatabaseConnect();
    void historyDatabaseInit();
    bool isExist();
    bool historyDatabaseDelete(int userID);
};

#endif // HISTORYDATABASEMANAGER_H
