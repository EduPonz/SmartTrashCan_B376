#include "trashinfodatabasemanager.h"
#include "userdatabasemanager.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>


TrashInfoDatabaseManager::TrashInfoDatabaseManager()
{
    TABLE_NAME = "trashInfo";
    COLUMN_ID = "id";
    COLUMN_TIME = "time";
    COLUMN_USERID = "user_id";
    COLUMN_FULLNESS = "fullness";
    COLUMN_WEIGHT = "weight";
    COLUMN_HUMIDITY = "humidity";
    COLUMN_TEMPERATURE = "temperature";
    COLUMN_TIME = "time";
    if(isExist()) {
        trashInfoDatabaseInit();
    }

}

bool TrashInfoDatabaseManager::isExist()
{
    if(!userDatabaseManager.userDataBase.open()){
        qWarning() << "TrashInfoDatabaseManager::isExist - ERROR: Cannot open the Database";
        return false;
    }else{
        qDebug() << "TrashInfoDatabaseManager::isExist - The database exists";
        qDebug() << "TrashInfoDatabaseManager::isExist - The database has been opened";
        return true;
    }
}

void TrashInfoDatabaseManager::fakeTrashInfo(int userId)
{
    //srand (time(NULL));
    int fullness = rand() % 100 + 1;
    int weight = rand() % 30 + 1;
    int humidity = rand() % 100 + 1;
    int temperature = rand() % 50 + 1;

    trashInfoDatabaseInsert(userId, fullness, weight, humidity, temperature);
}

void TrashInfoDatabaseManager::trashInfoDatabaseInit()
{
    QString SQL_CREATE_TRASH_INFO_DATABASE_TABLE = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " ("
            + COLUMN_ID          + " INTEGER PRIMARY KEY, "
            + COLUMN_USERID      + " INTEGER, "
            + COLUMN_TIME        + " TEXT, "
            + COLUMN_FULLNESS    + " REAL, "
            + COLUMN_WEIGHT      + " REAL, "
            + COLUMN_HUMIDITY    + " REAL, "
            + COLUMN_TEMPERATURE + " REAL)";

    QSqlQuery query(SQL_CREATE_TRASH_INFO_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "TrashInfoDatabaseManager::trashInfoDatabaseInit - ERROR: " << query.lastError().text();
}

QSqlQuery TrashInfoDatabaseManager::trashInfoDatabaseRetrieveMonthly(int userID)
{
    QSqlQuery query;

    QString time1 = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString time2 = QDateTime::currentDateTime().addMonths(-11).toString(Qt::ISODate);

    QString SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE = "SELECT "
            + COLUMN_TIME            + ", "
            + COLUMN_FULLNESS        + ", "
            + COLUMN_WEIGHT          + ", "
            + COLUMN_HUMIDITY        + ", "
            + COLUMN_TEMPERATURE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERID + " = ?"
            + " AND "
            + COLUMN_TIME   + " <= ?"
            + " AND "
            + COLUMN_TIME   + " >= ?";
    query.prepare(SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE);
    query.addBindValue(userID);
    query.addBindValue(time1);
    query.addBindValue(time2);

    if(!query.exec())
        qWarning() << "TrashInfoDataBaseManager::trashInfoDatabaseRetrieve - ERROR: " << query.lastError().text();
    return query;
}

QSqlQuery TrashInfoDatabaseManager::trashInfoDatabaseRetrieveDaily(int userID)
{
    QSqlQuery query;

    QString time1 = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString time2 = QDateTime::currentDateTime().addDays(-6).toString(Qt::ISODate);

    QString SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE = "SELECT "
            + COLUMN_TIME            + ", "
            + COLUMN_FULLNESS        + ", "
            + COLUMN_WEIGHT          + ", "
            + COLUMN_HUMIDITY        + ", "
            + COLUMN_TEMPERATURE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERID + " = ?"
            + " AND "
            + COLUMN_TIME   + " <= ?"
            + " AND "
            + COLUMN_TIME   + " >= ?";
    query.prepare(SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE);
    query.addBindValue(userID);
    query.addBindValue(time1);
    query.addBindValue(time2);

    if(!query.exec())
        qWarning() << "TrashInfoDataBaseManager::trashInfoDatabaseRetrieve - ERROR: " << query.lastError().text();
    return query;
}

bool TrashInfoDatabaseManager::trashInfoDatabaseInsert(int userID, float fullness, float weight, float humidity, float temperature){
    QSqlQuery query;
    QString date = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString SQL_POPULATE_TRASH_INFO_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME
            + " ("
            + COLUMN_USERID       + ", "
            + COLUMN_TIME         + ", "
            + COLUMN_FULLNESS     + ", "
            + COLUMN_WEIGHT       + ", "
            + COLUMN_HUMIDITY     + ", "
            + COLUMN_TEMPERATURE
            + ") VALUES (:userID, :date, :fullness, :weight, :humidity, :temperature)";
    query.prepare(SQL_POPULATE_TRASH_INFO_DATABASE_TABLE);
    query.addBindValue(userID);
    query.addBindValue(date);
    query.addBindValue(fullness);
    query.addBindValue(weight);
    query.addBindValue(humidity);
    query.addBindValue(temperature);

    if(!query.exec()){
        qDebug() << "TrashInfoDatabaseManager::trashInfoDatabaseInsert - : " << query.lastQuery();
        qWarning() << "TrashInfoDatabaseManager::trashInfoDatabaseInsert - ERROR: " << query.lastError().text();
        return false;
    }else{
        return true;
    }
}

bool TrashInfoDatabaseManager::trashInfoDatabaseDelete(int userID)
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
