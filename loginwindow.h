/*! \class LoginWindow class.
    \brief Hey Vincent whats up
    THIS IS MY FAVOURITE CLASS yayayaya
    ldksmdas,mdls;admasdad
    aksdmaslkdmalksdmalksdmclk lskenfirng
    askdnmioeneoisfnrwoifneroinoirngdio
    dklsemnfowienfoinfgeroinetoinboieng
    dsknfrsoinffeoriniodnmweifnorwingeroinf
    dkmsfoirengoineroginegoirngeroignergoie
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
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void correctUser(int id);
    void signUpSignal();

public slots:
    void on_loginButton_clicked(); /*!<Hey edu whatsasd up */
    void on_loginSingupButton_clicked();

private slots:
    void on_loginRemoveButton_clicked();

private:
    QString userNameEntry;
    QString passWordEntry;

    QSqlQuery loginQuery;
    UserDataBaseManager loginDataBaseManager;
    DataWindow *dataWindow;

    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
