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
    SignUpWindow(QWidget *parent = 0);
    ~SignUpWindow();

signals:
    void signUpAddUserSignal(QString userName);
    void singUpCancelSignUp();

private slots:
    void on_signUpSingUpButton_clicked();

    void on_signUpCancelButton_clicked();

private:
    QString userName;
    QString password;
    QString full_name;
    QString address;
    QString phone_number;
    QString payment_method;
    QString card_number;
    QString cvv;
    QString expiration_date;

    UserDataBaseManager signUpDatabaseManager;
    QSqlQuery signUpQuery;

    void createActions();
    void insertNewUser();

    LoginWindow *loginWindow;
    Ui::SignUpWindow *ui;
};

#endif // SIGNUP_H




