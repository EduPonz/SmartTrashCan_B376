#include "trashinfodatabasemanager.h"
#include "userdatabasemanager.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

TrashInfoDatabaseManager::TrashInfoDatabaseManager()
{
    TABLE_NAME = "trashInfo";
    COLUMN_ID = "id";
    COLUMN_USERID = "user_id";
    COLUMN_FULLNESS = "fullness";
    COLUMN_WEIGHT = "weight";
    COLUMN_HUMIDITY = "humidity";
    COLUMN_TEMPERATURE = "temperature";
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

void TrashInfoDatabaseManager::trashInfoDatabaseInit()
{
    QString SQL_CREATE_TRASH_INFO_DATABASE_TABLE = "CREATE TABLE " + TABLE_NAME + " ("
            + COLUMN_ID       + " INTEGER PRIMARY KEY, "
            + COLUMN_USERID + " INTEGER, "
            + COLUMN_FULLNESS + " REAL, "
            + COLUMN_WEIGHT + " REAL, "
            + COLUMN_HUMIDITY + " REAL, "
            + COLUMN_TEMPERATURE + " REAL)";

    QSqlQuery query(SQL_CREATE_TRASH_INFO_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "TrashInfoDatabaseManager::trashInfoDatabaseInit - ERROR: " << query.lastError().text();
    else
        qDebug() << "TrashInfoDatabaseManager::trashInfoDatabaseInit -  The DataBase has been inizialited";
}

QSqlQuery TrashInfoDatabaseManager::trashInfoDatabaseRetrieve(int userID)
{
    QSqlQuery query;

    QString SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE = "SELECT "
            + COLUMN_FULLNESS        + ", "
            + COLUMN_WEIGHT          + ", "
            + COLUMN_HUMIDITY        + ", "
            + COLUMN_TEMPERATURE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERID + " = '" + userID + "'";

    if(!query.exec(SQL_RETRIEVE_TRASH_INFO_DATABASE_TABLE))
        qWarning() << "TrashInfoDataBaseManager::trashInfoDatabaseRetrieve - ERROR: " << query.lastError().text();

    return query;
}

bool TrashInfoDatabaseManager::trashInfoDatabaseInsert(int userID, float fullness, float weight, float humidity, float temperature){
    QSqlQuery query;
    QString SQL_POPULATE_TRASH_INFO_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME
            + "("
            + COLUMN_USERID       + ", "
            + COLUMN_FULLNESS     + ", "
            + COLUMN_WEIGHT       + ", "
            + COLUMN_HUMIDITY     + ", "
            + COLUMN_TEMPERATURE
            + ") VALUES ('"
            + userID              + "', '"
            + fullness            + "', '"
            + weight              + "', '"
            + humidity            + "')";

    if(!query.exec(SQL_POPULATE_TRASH_INFO_DATABASE_TABLE)){
        qWarning() << "UserDataBaseManager::userDatabaseInsert - ERROR: " << query.lastError().text();
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
