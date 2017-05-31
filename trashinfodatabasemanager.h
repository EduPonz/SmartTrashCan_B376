/*! \class TrashInfoDatabaseManager
 *  \brief This class handles the trashInfo table in the SQlite database, which
    stores fullness, weight, humidity, temperature and time of the entry. It links the
    trash information with the user by  adding the atribute userid to the entries. This data
    is used to create the charts and current data in the DataWindow UI.
 * */

#ifndef TRASHINFODATABASEMANAGER_H
#define TRASHINFODATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include "userdatabasemanager.h"
#include <QSqlQuery>

class TrashInfoDatabaseManager
{
public:
    TrashInfoDatabaseManager(); /*!<Populates the table strings for the name and attributes. Initializes the database calling to trashInfoDatabaseInit()*/

    QString COLUMN_ID;
    QString COLUMN_USERID;
    QString COLUMN_FULLNESS;
    QString COLUMN_WEIGHT;
    QString COLUMN_HUMIDITY;
    QString COLUMN_TEMPERATURE;
    QString COLUMN_TIME;

    QSqlQuery trashInfoDatabaseRetrieveDaily(int userID); /*!<Retrieves userID trash information of the last 7 days.*/
    QSqlQuery trashInfoDatabaseRetrieveWeekly(int userID); /*!<Retrieves userID trash information of the last 7 weeks.*/
    QSqlQuery trashInfoDatabaseRetrieveMonthly(int userID); /*!<Retrieves userID trash information of the last 12 months.*/
    QSqlQuery trashInfoDatabaseRetrieveYearly(int userID); /*!<Retrieves userID trash information of the last 5 years.*/
    bool trashInfoDatabaseInsert(int userID, float fullness, float weight, float humidity, float temperature); /*!<Inserts a new trashInfo entry with a time and a userID attributes. Returns true if the writing operation successes.*/

private:
    void trashInfoDatabaseInit(); /*!<Creates a trashInfo table in the database if it does not exist aready.*/
    bool isExist(); /*!<Opens the database if it exists. Returns true if the writing operation successes.*/
    bool trashInfoDatabaseDelete(int userID); /*!<*Deletes all the trash information linked to the userID.*/

    QString TABLE_NAME;
    UserDataBaseManager userDatabaseManager;
};

#endif // TRASHINFODATABASEMANAGER_H
