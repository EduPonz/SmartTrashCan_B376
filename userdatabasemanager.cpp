#include "userdatabasemanager.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QDir>

UserDataBaseManager::UserDataBaseManager()
{
    //SQL_CREATE_USER_DATABASE_TABLE
    TABLE_NAME = "users";
    COLUMN_ID = "id";
    COLUMN_USERNAME = "username";
    COLUMN_PASSWORD = "password";
    COLUMN_FULL_NAME = "full_name";
    COLUMN_ADDRESS = "address";
    COLUMN_PHONE_NUMBER = "phone_number";
    COLUMN_PAYMENT_METHOD = "payment_method";
    COLUMN_CARD_NUMBER = "card_number";
    COLUMN_CVV = "cvv";
    COLUMN_EXPIRATION_DATE = "expiration_date";
    PAYMENT_METHOD_UNKNOWN = "unknown";
    PAYMENT_METHOD_VISA = "visa";
    PAYMENT_METHOD_MASTERCARD = "mastercard";
    PAYMENT_METHOD_BOTTLECAPS = "bottlecaps";
    NOT_PERSON_FOUND = "Sorry, wrong username or password!";
    USER_ADDED_SUCC = "New user added successfully";
    USER_ADDED_FAIL = "Unable to add the new user";
    USER_EXISTS = "Username not available. Try another username";
    userDatabaseConnect();
}

bool UserDataBaseManager::isExist()
{
    if(!userDataBase.open()){
        qWarning() << "UserDataBaseManager::isExist - ERROR: Cannot open the Database";
        return false;
    }else{
        qDebug() << "UserDataBaseManager::UserDataBaseManager - The database exists";
        qDebug() << "UserDataBaseManager::UserDataBaseManager - The database has been opened";
        return true;
    }
}

void UserDataBaseManager::userDatabaseConnect()
{
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QDir currentPath;
        userDatabasePath = currentPath.currentPath() + "smartTrashCanDatabe.SQL";
        qDebug() << "UserDataBaseManager::userDatabaseConnect - Database Path" << userDatabasePath;
        userDataBase = QSqlDatabase::addDatabase(DRIVER);
        userDataBase.setDatabaseName(userDatabasePath);
        if(isExist()) {
            userDatabaseInit();
        }
    }else{
        qWarning() << "UserDataBaseManager::userDatabaseConnect - ERROR: no driver " << DRIVER << " available";
    }
}

void UserDataBaseManager::userDatabaseInit()
{
    QString SQL_CREATE_USER_DATABASE_TABLE = "CREATE TABLE " + TABLE_NAME + " ("
            + COLUMN_ID       + " INTEGER PRIMARY KEY, "
            + COLUMN_USERNAME + " TEXT NOT NULL, "
            + COLUMN_PASSWORD + " TEXT NOT NULL, "
            + COLUMN_FULL_NAME + " TEXT, "
            + COLUMN_ADDRESS + " TEXT NOT NULL, "
            + COLUMN_PHONE_NUMBER + " TEXT, "
            + COLUMN_PAYMENT_METHOD + " TEXT, "
            + COLUMN_CARD_NUMBER + " TEXT, "
            + COLUMN_CVV + " TEXT, "
            + COLUMN_EXPIRATION_DATE + " TEXT)";

    QSqlQuery query(SQL_CREATE_USER_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "UserDataBaseManager::userDatabaseInit - ERROR: " << query.lastError().text();
    else
        qDebug() << "UserDataBaseManager::userDatabaseInit -  The DataBase has been inizialited";
}

bool UserDataBaseManager::availableUserName(QString userName){
    QSqlQuery query;
    QString SQL_AVAILABLE_USER_NAME = "SELECT " + COLUMN_USERNAME + " FROM "
            + TABLE_NAME;
    query.exec(SQL_AVAILABLE_USER_NAME);
    query.first();
    bool rtn;
    int i = 0;
    do {
        if (query.value(0).toString().compare(userName)){
            rtn = true;
        }else{
            rtn = false;
            break;
        }
    }while (query.next());
    return rtn;
}

bool UserDataBaseManager::availableUpdateUserName(int id, QString userNewName){

    QSqlQuery query;
    QString idString;
    qDebug() << "UserDataBaseManager::availableUpdateUserName - ID: " << idString.number(id);

    QString SQL_AVAILABLE_USER_NAME = "SELECT " + COLUMN_USERNAME
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID
            + " != " + idString.number(id);

    query.exec(SQL_AVAILABLE_USER_NAME);
    qDebug() << SQL_AVAILABLE_USER_NAME;

    query.first();
    bool available;
    int i = 0;
    do {
        if (query.value(0).toString().compare(userNewName)){
            available = true;
            i++;
        }else{
            available = false;
            break;
        }
    }while (query.next());
    return available;
}

bool UserDataBaseManager::userDataBaseUpdate(int userId, QString userName, QString password, QString full_name,
                                              QString address, QString phone_number, QString payment_method,
                                              QString card_number, QString cvv, QString expiration_date)
{
    QSqlQuery query;
    QString userIdString;

    qDebug () << "UserDataBaseManager::userDataBaseUpdate - trying to update WHERE "
                 + COLUMN_ID + " = " + userIdString.number(userId);

    QString SQL_UPDATE_USER_DATABASE_TABLE = "UPDATE " + TABLE_NAME + " SET "
            + COLUMN_USERNAME        + " = '" + userName        + "', "
            + COLUMN_PASSWORD        + " = '" + password        + "', "
            + COLUMN_FULL_NAME       + " = '" + full_name       + "', "
            + COLUMN_ADDRESS         + " = '" + address         + "', "
            + COLUMN_PHONE_NUMBER    + " = '" + phone_number    + "', "
            + COLUMN_PAYMENT_METHOD  + " = '" + payment_method  + "', "
            + COLUMN_CARD_NUMBER     + " = '" + card_number     + "', "
            + COLUMN_CVV             + " = '" + cvv             + "', "
            + COLUMN_EXPIRATION_DATE + " = '" + expiration_date + "'"
            + " WHERE "
            + COLUMN_ID + " = " + userIdString.number(userId);

    query.prepare(SQL_UPDATE_USER_DATABASE_TABLE);
    qDebug() << SQL_UPDATE_USER_DATABASE_TABLE;
    if(!query.exec()){
        qWarning() << "UserDataBaseManager::userDatabaseUpdate - ERROR: " << query.lastError().text();
        return false;
    }else{
        qDebug() << "UserDataBaseManager::userDatabaseUpdate - UPDATED:" << "\n"
                 << " User Name: "      << userName       << "\n"
                 << " Password: "       << password       << "\n"
                 << " Full_name: "      << full_name      << "\n"
                 << " Address: "        << address        << "\n"
                 << " Phone_number: "   << phone_number   << "\n"
                 << " Payment_method: " << payment_method << "\n"
                 << " Card_number: "    << card_number    << "\n"
                 << " CVV: "            << cvv            << "\n"
                 << " Expiration_date: "<< expiration_date;
        return true;
    }
}

bool UserDataBaseManager::userDatabaseInsert(QString userName, QString password, QString full_name,
                                             QString address, QString phone_number, QString payment_method,
                                             QString card_number, QString cvv, QString expiration_date)
{
    QSqlQuery query;
    QString SQL_POPULATE_USER_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME
            + "("   + COLUMN_USERNAME    + ", " + COLUMN_PASSWORD     + ", " + COLUMN_FULL_NAME
            + ", "  + COLUMN_ADDRESS     + ", " + COLUMN_PHONE_NUMBER + ", " + COLUMN_PAYMENT_METHOD
            + ", "  + COLUMN_CARD_NUMBER + ", " + COLUMN_CVV          + ", " + COLUMN_EXPIRATION_DATE
            + ") VALUES ('"
            + userName    + "', '" + password     + "', '" + full_name       + "', '"
            + address     + "', '" + phone_number + "', '" + payment_method  + "', '"
            + card_number + "', '" + cvv          + "', '" + expiration_date + "')";

    if(!query.exec(SQL_POPULATE_USER_DATABASE_TABLE)){
        qWarning() << "UserDataBaseManager::userDatabaseInsert - ERROR: " << query.lastError().text();
        return false;
    }else{
        qDebug() << "UserDataBaseManager::userDatabaseInsert - INSERTED:"
                 << " User Name: "    << userName
                 << " Password: " << password;
        return true;
    }
}

QSqlQuery UserDataBaseManager::userDataBaseRetrieve(QString userName, QString passWord)
{
    QSqlQuery query;

    //PASSES TWO STRINGS INTO PLACEHOLDER 'user' AND 'password'
    QString SQL_RETRIEVE_USER_DATABASE_TABLE = "SELECT "
            + COLUMN_ID              + ", "
            + COLUMN_USERNAME        + ", "
            + COLUMN_PASSWORD        + ", "
            + COLUMN_FULL_NAME       + ", "
            + COLUMN_ADDRESS         + ", "
            + COLUMN_PHONE_NUMBER    + ", "
            + COLUMN_PAYMENT_METHOD  + ", "
            + COLUMN_CARD_NUMBER     + ", "
            + COLUMN_CVV             + ", "
            + COLUMN_EXPIRATION_DATE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_USERNAME + " = '" + userName
            + "' AND "
            + COLUMN_PASSWORD + " = '" + passWord + "'";

    qDebug() << SQL_RETRIEVE_USER_DATABASE_TABLE;

    if(!query.exec(SQL_RETRIEVE_USER_DATABASE_TABLE))
        qWarning() << "UserDataBaseManager::userDataBaseRetrieve - ERROR: " << query.lastError().text();
    else
        query.first();
        QString idString = query.value(0).toString();
        qDebug() << "UserDataBaseManager::userDataBaseRetrieve - RETRIEVED:"
                 << " Id: "        << idString
                 << " User Name: " << userName
                 << " Password: "  << passWord;
    return query;
}

QSqlQuery UserDataBaseManager::userDataBaseRetrieve(int userId)
{
    QSqlQuery query;

    //PASSES TWO STRINGS INTO PLACEHOLDER 'user' AND 'password'
    QString SQL_RETRIEVE_USER_DATABASE_TABLE = "SELECT "
            + COLUMN_USERNAME        + ", "
            + COLUMN_PASSWORD        + ", "
            + COLUMN_FULL_NAME       + ", "
            + COLUMN_ADDRESS         + ", "
            + COLUMN_PHONE_NUMBER    + ", "
            + COLUMN_PAYMENT_METHOD  + ", "
            + COLUMN_CARD_NUMBER     + ", "
            + COLUMN_CVV             + ", "
            + COLUMN_EXPIRATION_DATE
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID + " = ?";

    query.prepare(SQL_RETRIEVE_USER_DATABASE_TABLE);
    query.addBindValue(userId);

    if(!query.exec())
        qWarning() << "UserDataBaseManager::userDataBaseRetrieve - ERROR: " << query.lastError().text();
    else
        qDebug() << "UserDataBaseManager::userDataBaseRetrieve - RETRIEVED:"
                 << " User Id: " << userId;
    return query;
}

QString UserDataBaseManager::userDataBaseRetrieveFullName(int id)
{
    QSqlQuery query;

    QString SQL_RETRIEVE_FULLNAME_DATABASE_TABLE = "SELECT "
            + COLUMN_FULL_NAME
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID + " = ?";

    query.prepare(SQL_RETRIEVE_FULLNAME_DATABASE_TABLE);
    query.addBindValue(id);

    QString full_name;
    if(!query.exec()){
        qWarning() << "UserDataBaseManager::userDataBaseRetrieveFullName - ERROR: "
                   << query.lastError().text();
    }else{
        query.first();
        full_name = query.value(0).toString();
    }

    return full_name;
}

QString UserDataBaseManager::userDataBaseRetrieveUserName(int id)
{
    QSqlQuery query;

    QString SQL_RETRIEVE_USERNAME_DATABASE_TABLE = "SELECT "
            + COLUMN_USERNAME
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID + " = ?";

    query.prepare(SQL_RETRIEVE_USERNAME_DATABASE_TABLE);
    query.addBindValue(id);

    QString username;
    if(!query.exec()){
        qWarning() << "UserDataBaseManager::userDataBaseRetrieveUserName - ERROR: "
                   << query.lastError().text();
    }else{
        query.first();
        username = query.value(0).toString();
    }

    return username;
}

void UserDataBaseManager::userDatabaseDeleteAll(){
    QSqlQuery query(userDataBase);
    query.clear();
    userDataBase.close();
    if (QFile::remove(userDatabasePath)){
        qDebug() << "UserDataBaseManager::userDatabaseDeleteAll() - Table" << TABLE_NAME << "removed";
    }else{
        qDebug() << "UserDataBaseManager::userDatabaseDeleteAll() - Table" << TABLE_NAME << "NOT removed";
    }
    userDatabaseConnect();
}
