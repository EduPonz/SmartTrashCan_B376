#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionswindow.h"
#include "loginwindow.h"
#include "datawindow.h"
#include "signupwindow.h"
#include "userdatabasemanager.h"
#include "extrapickupwindow.h"
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
     void englishSlot();
     void danishSlot();
     void lightThemeSlot();
     void darkThemeSlot();
     void normalUnitSlot();
     void americanUnitSlot();
     void updateWindowName(QString fullName);
     void extraPickUpSlot(int id);

private:
    void createActions();
    void createToolBars(int language);
    void createMenu();

    static const int ENGLISH_LANGUAGE = 1;
    static const int DANISH_LANGUAGE = 2;

    QAction *settingsAction;
    QAction *personalInfo;
    QAction *themeLight;
    QAction *themeDark;
    QAction *normalUnits;
    QAction *americanUnits;
    QAction *home;
    QAction *logOut;
    QAction *homeButtonAction;
    QAction *settingButton;
    QAction *danishAction;
    QAction *englishAction;

    QToolButton *themeButton;
    QToolButton *userNameButton;
    QToolButton *languageButton;
    QToolButton *unitsButton;

    int id;

    QIcon settingsIcon;

    QMenu *languageMenu;
    QMenu *userNameMenu;
    QMenu *themeMenu;
    QMenu *unitsMenu;

    OptionsWindow *optionsWindow;
    DataWindow *dataWindow;
    LoginWindow *loginWindow;
    SignUpWindow *signUpWindow;
    ExtraPickupWindow *extraPickupWindow;
    UserDataBaseManager mainWindowUserDatabaseManager;
    QSqlQuery mainWindowQuery;
};

#endif // MAINWINDOW_H
