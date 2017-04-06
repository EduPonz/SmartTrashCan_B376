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

UserDataBaseManager::UserDataBaseManager()
{

    //SQL_CREATE_USER_DATABASE_TABLE
    TABLE_NAME = "users";
    COLUMN_ID = "id";
    COLUMN_USERNAME = "username";
    COLUMN_PASSWORD = "password";
    NOT_PERSON_FOUND = "Sorry, wrong username or password!";
    DatabaseConnect();
}

bool UserDataBaseManager::isExist()
{
    if(!userDataBase.open()){
        qWarning() << "UserDataBaseManager::isExist - ERROR: "
                   << userDataBase.lastError().text();
        return false;
    }else{
        return true;
    }
}

void UserDataBaseManager::DatabaseConnect()
{
    const QString DRIVER("QSQLITE");

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        userDataBase = QSqlDatabase::addDatabase(DRIVER);
        //userDataBase.setDatabaseName(":/SmartTrashCan_B376/Databases/userDataBase.db");
        userDataBase.setDatabaseName(":memory:");
        if(isExist())
        {
            DatabaseInit();
        }else{
            DatabaseInit();
        }
    }else
        qWarning() << "UserDataBaseManager::DatabaseConnect - ERROR: no driver " << DRIVER << " available";
}

void UserDataBaseManager::DatabaseInit()
{

    QString SQL_CREATE_USER_DATABASE_TABLE = "CREATE TABLE " + TABLE_NAME + " ("
            + COLUMN_ID       + " INTEGER PRIMARY KEY, "
            + COLUMN_USERNAME + " TEXT, "
            + COLUMN_PASSWORD + " TEXT)";

    QSqlQuery query(SQL_CREATE_USER_DATABASE_TABLE);

    if(!query.isActive())
        qWarning() << "UserDataBaseManager::DatabaseInit - ERROR: " << query.lastError().text();
    else
        qDebug() << "UserDataBaseManager::DatabaseInit -  The DataBase has been inizialited";
    DatabasePopulate();
}

void UserDataBaseManager::DatabasePopulate() //PASS TWO STRINGS INTO PLACEHOLDER 'user' AND 'password'
{
    QSqlQuery query;

    QString SQL_POPULATE_USER_DATABASE_TABLE = "INSERT INTO " + TABLE_NAME + "("
            + COLUMN_USERNAME + ", " + COLUMN_PASSWORD + ") VALUES ('user', 'password')";

    if(!query.exec(SQL_POPULATE_USER_DATABASE_TABLE))
        qWarning() << "UserDataBaseManager::DatabasePopulate - ERROR: " << query.lastError().text();
    else
        qDebug() << "UserDataBaseManager::DatabasePopulate - The DataBase has been populated";
}

QString UserDataBaseManager::DataBaseRetrieve(QString userNameLineEdit, QString passWordLineEdit)
{
    QSqlQuery query;

    QString SQL_RETRIEVE_USER_DATABASE_TABLE = "SELECT "
            + COLUMN_USERNAME + ", "
            + COLUMN_PASSWORD + " FROM "
            + TABLE_NAME      + " WHERE "
            + COLUMN_USERNAME + " = ? AND "
            + COLUMN_PASSWORD + " = ?";

    query.prepare(SQL_RETRIEVE_USER_DATABASE_TABLE);
    query.addBindValue(userNameLineEdit);
    query.addBindValue(passWordLineEdit);

    if(!query.exec())
        qWarning() << "UserDataBaseManager::OnSearchClicked - ERROR: " << query.lastError().text();
    else
        qDebug() << "UserDataBaseManager::OnSearchClicked -"
                 << " User Name: " << userNameLineEdit
                 << "Password: "   << passWordLineEdit;

    if(query.first())
        return (query.value(0).toString());
    else
        return (NOT_PERSON_FOUND);
}
