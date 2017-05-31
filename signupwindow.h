#ifndef SIGNUP_H
#define SIGNUP_H

#include "userdatabasemanager.h"
#include "loginwindow.h"
#include <QWidget>
#include <QMainWindow>
#include <QSqlQuery>
#include <QString>

namespace Ui {
    class SignUpWindow;
}

class SignUpWindow : public QWidget
{
    Q_OBJECT

public:
    SignUpWindow(QWidget *parent = 0); /*!<Assigns the UI.*/
    ~SignUpWindow(); /*!<Deletes the signupwindow.ui UI.*/

signals:
    void signUpAddUserSignal(int id); /*!<Signal emitted when a new user has been added to the database.*/
    void singUpCancelSignUp(); /*!<Signal emitted when the signUpCancelButton button has been pressed.*/

private slots:
    void on_signUpSingUpButton_clicked(); /*!<Slot triggered when the signUpSingUpButton has been pressed. Calls to insertNewUser() if all the mandatory fields are filled and the username is available. Otherwise,set a message using signUpOutPutLabel.*/
    void on_signUpCancelButton_clicked(); /*!<Slot triggered when the signUpCancelButton has been pressed. Emits the signal singUpCancelSignUp().*/

private:
    void insertNewUser(); /*!<Uses the UserDataBaseManager.userDatabaseInsert method to insert a new user with the data provided in the SignUpWindow.*/

    QString userName;
    QString password;
    QString full_name;
    QString address;
    QString phone_number;
    QString email;
    QString payment_method;
    QString card_number;
    QString cvv;
    QString expiration_date;

    UserDataBaseManager signUpDatabaseManager;
    QSqlQuery signUpQuery;

    LoginWindow *loginWindow;
    Ui::SignUpWindow *ui;
};

#endif // SIGNUP_H




