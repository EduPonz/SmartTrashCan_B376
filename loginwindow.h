#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "userdatabasemanager.h"
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

private slots:
    void on_loginButton_clicked();

private:
    QString userName;
    UserDataBaseManager query;

    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
