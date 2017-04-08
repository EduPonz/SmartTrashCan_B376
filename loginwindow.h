#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "userdatabasemanager.h"
#include"datawindow.h"
#include <QWidget>
#include <QString>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void correctUser(QString userName);

public slots:
    void on_loginButton_clicked();
    void on_loginSingupButton_clicked();

private:
    QString userNameEntry;
    QString passWordEntry;
    QString databaseUserName;
    UserDataBaseManager query;

    DataWindow *dataWindow;

    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
