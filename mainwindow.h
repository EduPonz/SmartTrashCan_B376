/*! \class MainWindow class.
    \brief This class controls the main application UI: mainwindow.ui. This
    mainwindow.ui provides a Toolbar with different actions, and a placeholder
    which is filled with different UIs deppending on the user's behaviuor. This class
    handles the aplication Toolbar, as well as which UI is to be displayed.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionswindow.h"
#include "loginwindow.h"
#include "datawindow.h"
#include "signupwindow.h"
#include "userdatabasemanager.h"
#include "trashinfodatabasemanager.h"
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
    MainWindow(QWidget *parent = 0); /*!<Sets the UI to mainwindow.ui. Creates objects of LoginWindow, SignUpWindow and DataWindow. Sets its central widget to be a LoginWindow. Creates a sets the application Toolbar. Creates the connections needed to control the application.*/
    ~MainWindow(); /*!<Deletes the mainwindow.ui UI.*/
    QToolBar *mainToolBar;
    Ui::MainWindow *ui;
    void setNameWindowName(); /*!<Sets the userNameButton action text to be the user's full name.*/

private slots:
     void personalInfoSlot(); /*!<Slot triggered when the user clicks on the settingButton action of the Toolbar. Sets the central widget to be a OptionsWindow. Sets the toolbar to invisible. Sets the window title to "Personal Information". Establishes the required connections.*/
     void logOutSlot(); /*!<Slot triggered when the user clicks on the logOut action of the Toolbar. Sets the central widget to be a LoginWindow. Establishes the required connections.*/
     void homeButtonActionSlot(); /*!<Slot triggered when the user clicks on the homeButtonAction action of the Toolbar. Sets the central widget to be a DataWindow. Sets the toolbar to visible. Sets the window title to "Personal Data". Establishes the required connections.*/
     void accessDataSlot(int userId); /*!<Slot triggered when a new user has been added to the database. Sets the central widget to be a DataWindow. Sets the toolbar to visible. Sets the window title to "Personal Data". Establishes the required connections.*/
     void signUpSlot(); /*!<Slot triggered when the user clicks on the loginSingupButton button of the LoginWindow. Sets the central widget to be a DataWindow. Sets the toolbar to invisible. Sets the window title to "Sign Up". Establishes the required connections.*/
     //void englishSlot();
     //void danishSlot();
     //void lightThemeSlot();
     //void darkThemeSlot();
     //void normalUnitSlot();
     //void americanUnitSlot();
     void updateWindowName(QString fullName); /*!<Slot triggered when the user updates their profile in the OptionsWindow. Sets the userNameButton action text to be the new user's full name.*/
     void extraPickUpSlot(int id); /*!<Slot triggered when the user clicks on the datawindowPickupButton button of the DataWindow. Sets the central widget to be an ExtraPickupWindow. Sets the toolbar to visible. Sets the window title to "Personal Data". Establishes the required connections.*/

private:
    void createActions(); /*!<Creates all the Toolbar actions and the required connections to operate the Toolbar and the central widget.*/
    void createToolBars(int language); /*!<Creates the Toolbar and displays the actions in it.*/
    void createMenu(); /*!<Creates the user's menu.*/

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
    TrashInfoDatabaseManager mainWindowTrashInfoDatabaseManager;
    QSqlQuery mainWindowQuery;
};

#endif // MAINWINDOW_H
