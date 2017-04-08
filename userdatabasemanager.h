#ifndef USERDATABASEMANAGER_H
#define USERDATABASEMANAGER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>


class UserDataBaseManager
{
public:
    UserDataBaseManager();

    QString NOT_PERSON_FOUND;
    QString USER_ADDED_SUCC;
    QString USER_EXISTS;

    QString userDataBaseRetrieve(QString userNameLineEdit, QString passWordLineEdit);
    void userDatabaseInsert(QString userName, QString password);

private:

    QString TABLE_NAME;
    QString COLUMN_ID;
    QString COLUMN_USERNAME;
    QString COLUMN_PASSWORD;
    QSqlDatabase userDataBase;

    void DatabaseConnect();
    void DatabaseInit();

    bool isExist();
};

#endif // USERDATABASEMANAGER_H
