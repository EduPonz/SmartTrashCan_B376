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
    QString USER_ADDED_FAIL;
    QString USER_EXISTS;
    QString PAYMENT_METHOD_UNKNOWN;
    QString PAYMENT_METHOD_VISA;
    QString PAYMENT_METHOD_MASTERCARD;
    QString PAYMENT_METHOD_BOTTLECAPS;

    QSqlQuery userDataBaseRetrieve(QString userNameLineEdit, QString passWordLineEdit);
    bool userDatabaseInsert(QString userName, QString password, QString full_name,
                            QString address, QString phone_number, QString payment_method,
                            QString card_number, QString cvv, QString expiration_date);
    void userDatabaseDeleteAll();

private:
    QString TABLE_NAME;
    QString COLUMN_ID;
    QString COLUMN_USERNAME;
    QString COLUMN_PASSWORD;
    QString COLUMN_FULL_NAME;
    QString COLUMN_ADDRESS;
    QString COLUMN_PHONE_NUMBER;
    QString COLUMN_PAYMENT_METHOD;
    QString COLUMN_CARD_NUMBER;
    QString COLUMN_CVV;
    QString COLUMN_EXPIRATION_DATE;
    QString userDatabasePath;

    QSqlDatabase userDataBase;

    void userDatabaseConnect();
    void userDatabaseInit();

    bool isExist();
};

#endif // USERDATABASEMANAGER_H
