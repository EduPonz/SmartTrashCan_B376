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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    COLUMN_EMAIL = "email";
    COLUMN_PAYMENT_METHOD = "payment_method";
    COLUMN_CARD_NUMBER = "card_number";
    COLUMN_CVV = "cvv";
    COLUMN_EXPIRATION_DATE = "expiration_date";
    COLUMN_PICKUP_DAY = "pickup_day";
    PAYMENT_METHOD_UNKNOWN = "unknown";
    PAYMENT_METHOD_VISA = "visa";
    PAYMENT_METHOD_MASTERCARD = "mastercard";
    PAYMENT_METHOD_BOTTLECAPS = "bottlecaps";
    NOT_PERSON_FOUND = "Sorry, wrong username or password!";
    USER_ADDED_SUCC = "New user added successfully";
    USER_ADDED_FAIL = "Unable to add the new user";
    USER_EXISTS = "Username not available. Try another username";
    MONDAY = "monday";
    TUESDAY = "tuesday";
    WEDNESDAY = "wednesday";
    THURSDAY = "thursday";
    FRIDAY = "friday";
    userDatabaseConnect();
}

bool UserDataBaseManager::isExist()
{
    if(!userDataBase.open()){
        qWarning() << "UserDataBaseManager::isExist - ERROR: Cannot open the Database";
        return false;
    }else{
        qDebug() << "UserDataBaseManager::isExist - The database exists";
        qDebug() << "UserDataBaseManager::isExist - The database has been opened";
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
    QString SQL_CREATE_USER_DATABASE_TABLE = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + " ("
            + COLUMN_ID       + " INTEGER PRIMARY KEY, "
            + COLUMN_USERNAME + " TEXT NOT NULL, "
            + COLUMN_PASSWORD + " TEXT NOT NULL, "
            + COLUMN_FULL_NAME + " TEXT, "
            + COLUMN_ADDRESS + " TEXT NOT NULL, "
            + COLUMN_PHONE_NUMBER + " TEXT, "
            + COLUMN_EMAIL + " TEXT, "
            + COLUMN_PAYMENT_METHOD + " TEXT, "
            + COLUMN_CARD_NUMBER + " TEXT, "
            + COLUMN_CVV + " TEXT, "
            + COLUMN_EXPIRATION_DATE + " TEXT, "
            + COLUMN_PICKUP_DAY + " TEXT)";

    QSqlQuery query(SQL_CREATE_USER_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "UserDataBaseManager::userDatabaseInit - ERROR: " << query.lastError().text();
}

bool UserDataBaseManager::availableUserName(QString userName){
    QSqlQuery query;
    QString SQL_AVAILABLE_USER_NAME = "SELECT " + COLUMN_USERNAME + " FROM "
            + TABLE_NAME;
    query.exec(SQL_AVAILABLE_USER_NAME);
    query.first();
    bool rtn;
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
                                              QString address, QString phone_number, QString email, QString payment_method,
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
            + COLUMN_EMAIL           + " = '" + email           + "', "
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
                 << " Email: "          << email          << "\n"
                 << " Payment_method: " << payment_method << "\n"
                 << " Card_number: "    << card_number    << "\n"
                 << " CVV: "            << cvv            << "\n"
                 << " Expiration_date: "<< expiration_date;
        return true;
    }
}

bool UserDataBaseManager::userDatabaseInsert(QString userName, QString password, QString full_name,
                                             QString address, QString phone_number, QString email, QString payment_method,
                                             QString card_number, QString cvv, QString expiration_date)
{
    /* generate secret number between 1 and 5: for the day of the week(mon->fri) for trash pickup day*/
    srand (time(NULL));
    int pickup_day = rand() % 5 + 1;
    QString pickup_day_string;
    switch (pickup_day) {
    case 1:
        pickup_day_string = MONDAY;
        break;
    case 2:
        pickup_day_string = TUESDAY;
        break;
    case 3:
        pickup_day_string = WEDNESDAY;
        break;
    case 4:
        pickup_day_string = THURSDAY;
        break;
    case 5:
        pickup_day_string = FRIDAY;
        break;
    default:
        break;
    }

    qDebug() << "UserDataBaseManager::userDatabaseInsert - RANDOM NUMBER:" << pickup_day;
    QSqlQuery query;
    QString SQL_POPULATE_USER_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME
            + "("  + COLUMN_USERNAME        + ", " + COLUMN_PASSWORD     + ", " + COLUMN_FULL_NAME
            + ", " + COLUMN_ADDRESS         + ", " + COLUMN_PHONE_NUMBER + ", " + COLUMN_EMAIL
            + ", " + COLUMN_PAYMENT_METHOD  + ", " + COLUMN_CARD_NUMBER  + ", " + COLUMN_CVV
            + ", " + COLUMN_EXPIRATION_DATE + ", " + COLUMN_PICKUP_DAY
            + ") VALUES ('"
            + userName        + "', '" + password     + "', '" + full_name + "', '"
            + address         + "', '" + phone_number + "', '" + email     + "', '"
            + payment_method  + "', '" + card_number  + "', '" + cvv       + "', '"
            + expiration_date + "', '" + pickup_day_string   + "')";

    if(!query.exec(SQL_POPULATE_USER_DATABASE_TABLE)){
        qWarning() << "UserDataBaseManager::userDatabaseInsert - ERROR: " << query.lastError().text();
        return false;
    }else{
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
            + COLUMN_EMAIL           + ", "
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
            + COLUMN_EMAIL           + ", "
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

QString UserDataBaseManager::userDataBaseRetrieveUserEmail(int id){

    QSqlQuery query;

    QString SQL_RETRIEVE_USER_DATABASE_EMAIL = "SELECT "
            + COLUMN_EMAIL
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID + " = ?";

    query.prepare(SQL_RETRIEVE_USER_DATABASE_EMAIL);
    query.addBindValue(id);
    qDebug() << "UserDataBaseManager::userDataBaseRetrieveUserEmail - The ID is: " << id;
    qDebug() << "UserDataBaseManager::userDataBaseRetrieveUserEmail - ERROR: " << query.lastQuery();

    if(!query.exec())
        qWarning() << "UserDataBaseManager::userDataBaseRetrieveUserEmail - ERROR: " << query.lastError().text();
    else{
        query.first();
        emailAddress = query.value(0).toString();
        qDebug() << "UserDataBaseManager::userDataBaseRetrieveUserEmail - RETRIEVED:"
                 << " User Id: " << id
                 << " Email Addres: " << emailAddress;
    }
    return emailAddress;
}

int UserDataBaseManager::userDataBaseRetrievePickupDay(int id)
{

    QSqlQuery query;

    QString SQL_RETRIEVE_USER_DATABASE_PICKUP_DAY = "SELECT "
            + COLUMN_PICKUP_DAY
            + " FROM "
            + TABLE_NAME
            + " WHERE "
            + COLUMN_ID + " = ?";

    query.prepare(SQL_RETRIEVE_USER_DATABASE_PICKUP_DAY);
    query.addBindValue(id);
    int pickup_day;
    if(!query.exec())
        qWarning() << "UserDataBaseManager::userDataBaseRetrievePickupDay - ERROR: " << query.lastError().text();
    else{
        query.first();
        pickup_day = query.value(0).toInt();

    }
    return pickup_day;
}


void UserDataBaseManager::userDatabaseDeleteAll()
{
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

bool UserDataBaseManager::userDatabaseDeleteUser(int id)
{
    QSqlQuery query;
    QString deleteUser = "DELETE FROM " + TABLE_NAME + " WHERE " + COLUMN_ID + " = ?";
    query.prepare(deleteUser);
    query.addBindValue(id);

    if (!query.exec()){
        qWarning() << "UserDataBaseManager::userDatabaseDeleteUser - ERROR: "
                   << query.lastError().text();
        return false;
    }else{
        qDebug() << "UserDataBaseManager::userDatabaseDeleteUser - USER with ID = " << id << " DELETED";
        return true;
    }
}
