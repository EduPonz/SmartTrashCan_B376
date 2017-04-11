#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionswindow.h"
#include "loginwindow.h"
#include "datawindow.h"
#include "signupwindow.h"
#include "userdatabasemanager.h"
#include <QMainWindow>
#include <QMenu>
#include <QAbstractButton>
#include <QtWidgets>
#include <QString>
#include <QSqlQuery>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QToolBar *mainToolBar;
    Ui::MainWindow *ui;
    void setNameWindowName();

private slots:
     void personalInfoSlot();
     void logOutSlot();
     void homeButtonActionSlot();
     void accessDataSlot(int userId);
     void signUpSlot();
     void updateWindowName(QString fullName);

private:
    void createActions();
    void createToolBars();
    void createMenu();

    QAction *settingsAction;
    QAction *personalInfo;
    QAction *language;
    QAction *theme;
    QAction *units;
    QAction *home;
    QAction *logOut;
    QAction *homeButtonAction;
    QAction *settingButton;


    int id;

    QIcon settingsIcon;

    QMenu *settingsMenu;
    QMenu *userNameMenu;

    QToolButton *userNameButton;

    OptionsWindow *optionsWindow;
    DataWindow *dataWindow;
    LoginWindow *loginWindow;
    SignUpWindow *signUpWindow;
    UserDataBaseManager mainWindowUserDatabaseManager;
    QSqlQuery mainWindowQuery;
};

#endif // MAINWINDOW_H
