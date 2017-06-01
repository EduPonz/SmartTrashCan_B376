/*! \class UserDataBaseManager class.
    \brief This class handles the users table in the SQLite database, which
    stores user information, payment detailes and pick up weekday.
*/

#ifndef USERDATABASEMANAGER_H
#define USERDATABASEMANAGER_H

#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>


class UserDataBaseManager
{
public:
    UserDataBaseManager();

    QString emailAddress;

    QString TABLE_NAME;

    QString NOT_PERSON_FOUND;
    QString USER_ADDED_SUCC;
    QString USER_ADDED_FAIL;
    QString USER_EXISTS;
    QString PAYMENT_METHOD_UNKNOWN;
    QString PAYMENT_METHOD_VISA;
    QString PAYMENT_METHOD_MASTERCARD;
    QString PAYMENT_METHOD_BOTTLECAPS;

    QString MONDAY;
    QString TUESDAY;
    QString WEDNESDAY;
    QString THURSDAY;
    QString FRIDAY;

    QString COLUMN_ID;
    QString COLUMN_USERNAME;
    QString COLUMN_PASSWORD;
    QString COLUMN_FULL_NAME;
    QString COLUMN_ADDRESS;
    QString COLUMN_PHONE_NUMBER;
    QString COLUMN_EMAIL;
    QString COLUMN_PAYMENT_METHOD;
    QString COLUMN_CARD_NUMBER;
    QString COLUMN_CVV;
    QString COLUMN_EXPIRATION_DATE;
    QString COLUMN_PICKUP_DAY;

    QSqlDatabase userDataBase;

    bool userDataBaseUpdate(int userId, QString userName, QString password, QString full_name,
                             QString address, QString phone_number, QString email, QString payment_method,
                             QString card_number, QString cvv, QString expiration_date); /*!<Updates the user details related to a specific user ID. */

    QSqlQuery userDataBaseRetrieve(QString userName, QString passWord); /*!<Uses the username and password to retrieve the user ID from the database. */
    QSqlQuery userDataBaseRetrieve(int userId); /*!<Uses the user ID to retrieve that user's personal info from the database. */
    bool userDatabaseInsert(QString userName, QString password, QString full_name,
                            QString address, QString phone_number, QString email, QString payment_method,
                            QString card_number, QString cvv, QString expiration_date); /*!<Generate a random number to create a trash collection day for that user, populates the user database with the user information and returns true if the operation was successful. */
    QString userDataBaseRetrieveFullName(int id); /*!<Uses the user ID to retrieve that user's full name from the database. */
    QString userDataBaseRetrieveUserName(int id); /*!<Uses the user ID to retrieve that user's username from the database. */
    QString userDataBaseRetrieveUserEmail(int id); /*!<Uses the user ID to retrieve that user's email from the database. */
    int userDataBaseRetrievePickupDay(int id); /*!<Uses the user ID to retrieve that user's next trash collection date. */
    bool availableUserName(QString userName); /*!<Compares the username entered by a new user with the usernames in the database to verify if it already exists and returns true or false. */
    bool availableUpdateUserName(int id, QString userNewName); /*!<Compares the username updates by an existing user with the usernames in the database to verify if it already exists and returns true or false. */
    void userDatabaseDeleteAll(); /*!<Removes the entire users database. */
    bool userDatabaseDeleteUser(int id); /*!<Removes a specific user from the users database and returns true if the user databse for a specific user ID has been deleted. */


private:
    QString userDatabasePath;

    void userDatabaseConnect(); /*!<Connects with the smartTrashCanDatabe.SQL database. */
    void userDatabaseInit(); /*!<Initializes the user databse table. */

    bool isExist(); /*!<Verifies if the user database exists and has been opened and returns true or false. */
};

#endif // USERDATABASEMANAGER_H
