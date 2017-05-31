/*! \class LoginWindow class.
    \brief This class controls the loginwindow.ui UI. I enables the user to introduce their
    login, load the signupwindow.ui UI and delete the database (for development reasons). It also checks
    the validity of the login and acts accordingly; emitting a signal to load the datawindow.ui UI or
    writting into a label.
*/


#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "userdatabasemanager.h"
#include "datawindow.h"
#include <QWidget>
#include <QString>
#include <QSqlQuery>

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0); /*!<Assigns the UI. Connects the username and password fields to the loginButton so when return is pressed the button emits signal clicked().*/
    ~LoginWindow(); /*!<Deletes the loginwindow.ui UI.*/

signals:
    void correctUser(int id); /*!<Signal emitted when the user and password match with an entry on the database. The signal carries the user ID.*/
    void signUpSignal(); /*!<Signal emitted when the loginSingupButton is pressed.*/

public slots:
    void on_loginButton_clicked(); /*!<Slot triggered when the loginButton is clicked. Check whether the user and password match with an entry on the database. If they do it emits signal correctUser(int id). If they do not, sets outPutLabel text to NOT_PERSON_FOUND.*/
    void on_loginSingupButton_clicked(); /*!<Slot triggerer when the SingupButton is clicked. Emits signal signUpSignal().*/

private slots:
    void on_loginRemoveButton_clicked(); /*!<Slot triggered when the loginRemoveButton is clicked. Calls to the UserDataBaseManager.userDatabaseDeleteAll() method to delete the entire database.*/

private:
    QString userNameEntry;
    QString passWordEntry;

    QSqlQuery loginQuery;
    UserDataBaseManager loginDataBaseManager;
    DataWindow *dataWindow;

    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
