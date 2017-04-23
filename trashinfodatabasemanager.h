#ifndef TRASHINFODATABASEMANAGER_H
#define TRASHINFODATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include "userdatabasemanager.h"
#include <QSqlQuery>

class TrashInfoDatabaseManager
{
public:
    TrashInfoDatabaseManager();

    QString COLUMN_ID;
    QString COLUMN_USERID;
    QString COLUMN_FULLNESS;
    QString COLUMN_WEIGHT;
    QString COLUMN_HUMIDITY;
    QString COLUMN_TEMPERATURE;
    QString COLUMN_TIME;

    void fakeTrashInfo(int userId);
    QSqlQuery trashInfoDatabaseRetrieveDaily(int userID);
    QSqlQuery trashInfoDatabaseRetrieveWeekly(int userID);
    QSqlQuery trashInfoDatabaseRetrieveMonthly(int userID);
    QSqlQuery trashInfoDatabaseRetrieveYearly(int userID);
    bool trashInfoDatabaseInsert(int userID, float fullness, float weight, float humidity, float temperature);

private:
    QString TABLE_NAME;

    UserDataBaseManager userDatabaseManager;

   // void trashInfoDatabaseConnect();
    void trashInfoDatabaseInit();
    bool isExist();
    bool trashInfoDatabaseDelete(int userID);

};

#endif // TRASHINFODATABASEMANAGER_H
