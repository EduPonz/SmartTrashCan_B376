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
