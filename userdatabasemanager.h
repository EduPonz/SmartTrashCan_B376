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

    QString DataBaseRetrieve(QString userNameLineEdit, QString passWordLineEdit);

private:

    QString TABLE_NAME;
    QString COLUMN_ID;
    QString COLUMN_USERNAME;
    QString COLUMN_PASSWORD;
    QSqlDatabase userDataBase;

    void DatabaseConnect();
    void DatabaseInit();
    void DatabasePopulate();

    bool isExist();
};

#endif // USERDATABASEMANAGER_H
