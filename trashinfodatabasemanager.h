#ifndef TRASHINFODATABASEMANAGER_H
#define TRASHINFODATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include "userdatabasemanager.h"

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

private:
    QString TABLE_NAME;
    QString trashInfoDatabasePath;

    UserDataBaseManager userDatabaseManager;

   // void trashInfoDatabaseConnect();
    void trashInfoDatabaseInit();
    bool isExist();

};

#endif // TRASHINFODATABASEMANAGER_H
