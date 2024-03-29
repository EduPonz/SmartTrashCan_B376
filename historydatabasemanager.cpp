#include "historydatabasemanager.h"
#include <QDebug>
#include <QSqlError>

HistoryDatabaseManager::HistoryDatabaseManager()
{
    TABLE_NAME = "historyData";
    COLUMN_ID = "id";
    COLUMN_USERID = "user_id";

    COLUMN_SELECTED_ITEM_SMALL = "bool_item_small";
    COLUMN_SELECTED_ITEM_MEDIUM = "bool_medium_small";
    COLUMN_SELECTED_ITEM_BIG = "bool_big_small";

    COLUMN_ITEM_SMALL_QUANTITY = "item_small_quantity"; //1-9
    COLUMN_ITEM_MEDIUM_QUANTITY = "item_medium_quantity"; //1-9
    COLUMN_ITEM_BIG_QUANTITY = "item_big_quantity"; //1-9

    COLUMN_COMMENTS = "comments";
    COLUMN_PICKUP_TIME = "pickup_price"; //1-3
    COLUMN_ITEM_PRICE = "item_price";
    COLUMN_TIME_PRICE = "time_price";
    COLUMN_TOTAL_PRICE = "total_price";

    COLUMN_PICK_UP_DATE = "pickup_date";
    COLUMN_INVOICE = "invoice";

    if(isExist()) {
        historyDatabaseInit();
    }
}

bool HistoryDatabaseManager::isExist()
{
    if(!userDatabaseManager.userDataBase.open()){
        qWarning() << "HistoryDatabaseManager::isExist - ERROR: Cannot open the Database";
        return false;
    }else{
        qDebug() << "HistoryDatabaseManager::isExist - The database exists";
        qDebug() << "HistoryDatabaseManager::isExist - The database has been opened";
        return true;
    }
}

void HistoryDatabaseManager::historyDatabaseInit()
{
    QString SQL_CREATE_HISTORY_DATABASE_TABLE = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " ("
            + COLUMN_ID       + " INTEGER PRIMARY KEY, "
            + COLUMN_USERID + " INTEGER, "
            + COLUMN_SELECTED_ITEM_SMALL + " BOOLEAN, "
            + COLUMN_SELECTED_ITEM_MEDIUM + " BOOLEAN, "
            + COLUMN_SELECTED_ITEM_BIG + " BOOLEAN, "
            + COLUMN_ITEM_SMALL_QUANTITY + " INTEGER, "
            + COLUMN_ITEM_MEDIUM_QUANTITY + " INTEGER, "
            + COLUMN_ITEM_BIG_QUANTITY + " INTEGER, "
            + COLUMN_COMMENTS + " REAL, "
            + COLUMN_PICKUP_TIME + " INTEGER, "
            + COLUMN_ITEM_PRICE + " INTEGER, "
            + COLUMN_TIME_PRICE + " INTEGER, "
            + COLUMN_TOTAL_PRICE + " INTEGER, "
            + COLUMN_PICK_UP_DATE + " TEXT, "
            + COLUMN_INVOICE + " INTEGER)";

    QSqlQuery query(SQL_CREATE_HISTORY_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "HistoryDatabaseManager::historyDatabaseInit - ERROR: " << query.lastError().text();
}

QSqlQuery HistoryDatabaseManager::historyDatabaseRetrieve(int userID)
{
    QSqlQuery query;

    QString SQL_RETRIEVE_HISTORY_DATABASE_TABLE = "SELECT "
            + COLUMN_SELECTED_ITEM_SMALL + ", "
            + COLUMN_SELECTED_ITEM_MEDIUM + ", "
            + COLUMN_SELECTED_ITEM_BIG + ", "
            + COLUMN_ITEM_SMALL_QUANTITY + ", "
            + COLUMN_ITEM_MEDIUM_QUANTITY + ", "
            + COLUMN_ITEM_BIG_QUANTITY + ", "
            + COLUMN_COMMENTS + ", "
            + COLUMN_PICKUP_TIME + ", "
            + COLUMN_ITEM_PRICE + " , "
            + COLUMN_TIME_PRICE + ", "
            + COLUMN_TOTAL_PRICE + ", "
            + COLUMN_PICK_UP_DATE + ", "
            + COLUMN_INVOICE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERID + " = ?";
    query.prepare(SQL_RETRIEVE_HISTORY_DATABASE_TABLE);
    query.addBindValue(userID);


    if(!query.exec())
        qWarning() << "HistoryDatabaseManager::historyDatabaseRetrieve - ERROR: " << query.lastError().text();

    return query;
}


QSqlQuery HistoryDatabaseManager::rowNumberRetrieve(int userID)
{
    QSqlQuery query;
    rows = 0;
    QString SQL_RETRIEVE_ROW_NUMBER_TABLE = "SELECT "
            + COLUMN_USERID
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERID + " = ?";

    query.prepare(SQL_RETRIEVE_ROW_NUMBER_TABLE);
    query.addBindValue(userID);

    if(!query.exec())
        qWarning() << "HistoryDatabaseManager::rowNumberRetrieve - ERROR: " << query.lastError().text();
    else if (query.first()){
            rows ++;
        while (query.next()) {
           rows ++;
        }
        qDebug() << "HistoryDatabaseManager::rowNumberRetrieve - NUMBER OF ROWS" << rows;
    }

    return query;

}
bool HistoryDatabaseManager::historyDatabaseInsert(int userID, bool selected_small, bool selected_medium, bool selected_big,
                                                   int small_qty, int medium_qty, int big_qty, QString comms, int pickup_time,
                                                   int item_price, int time_price, int total_price, QString time, int invoice){

    qDebug() << "HistoryDatabaseManager::historyDatabaseInsert - TIME AND INVOICE: " << time << ", " << invoice;
    QSqlQuery query;
    QString SQL_POPULATE_TRASH_INFO_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME
            + " ("
            + COLUMN_USERID               + ", "
            + COLUMN_SELECTED_ITEM_SMALL  + ", "
            + COLUMN_SELECTED_ITEM_MEDIUM + ", "
            + COLUMN_SELECTED_ITEM_BIG    + ", "
            + COLUMN_ITEM_SMALL_QUANTITY  + ", "
            + COLUMN_ITEM_MEDIUM_QUANTITY + ", "
            + COLUMN_ITEM_BIG_QUANTITY    + ", "
            + COLUMN_COMMENTS             + ", "
            + COLUMN_PICKUP_TIME          + ", "
            + COLUMN_ITEM_PRICE           + ", "
            + COLUMN_TIME_PRICE           + ", "
            + COLUMN_TOTAL_PRICE          + ", "
            + COLUMN_PICK_UP_DATE         + ", "
            + COLUMN_INVOICE
            + ") VALUES (:userID, :selected_small, :selected_medium, :selected_big, :small_qty, :medium_qty, :big_qty,"
              " :comms, :pickup_time, :item_price, :time_price, :total_price, :time, :invoice)";

    query.prepare(SQL_POPULATE_TRASH_INFO_DATABASE_TABLE);
    query.addBindValue(userID);
    query.addBindValue(selected_small);
    query.addBindValue(selected_medium);
    query.addBindValue(selected_big);
    query.addBindValue(small_qty);
    query.addBindValue(medium_qty);
    query.addBindValue(big_qty);
    query.addBindValue(comms);
    query.addBindValue(pickup_time);
    query.addBindValue(item_price);
    query.addBindValue(time_price);
    query.addBindValue(total_price);
    query.addBindValue(time);
    query.addBindValue(invoice);

    if(!query.exec()){
        qDebug() << "HistoryDatabaseManager::historyDatabaseInsert - Last Query\n "
                 << query.lastQuery();
        qWarning() << "HistoryDatabaseManager::historyDatabaseInsert - ERROR: " << query.lastError().text();
        return false;
    }else{
        return true;
    }
}

bool HistoryDatabaseManager::historyDatabaseDelete(int userID)
{
    QSqlQuery query;
    QString deleteUserTrashInfo = "DELETE FROM " + TABLE_NAME + " WHERE " + COLUMN_USERID + " = ?";
    query.prepare(deleteUserTrashInfo);
    query.addBindValue(userID);

    if (!query.exec()){
        qWarning() << "TrashInfoDatabaseManager::trashInfoDatabaseDelete - ERROR: "
                   << query.lastError().text();
        return false;
    }else{
        qDebug() << "TrashInfoDatabaseManager::trashInfoDatabaseDelete - USER with userID = " << userID << " DELETED";
        return true;
    }
}
