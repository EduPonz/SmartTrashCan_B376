#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_optionswindow.h"
#include "optionswindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"

#include <QtGui>
#include <QString>
#include <QToolBar>
#include <QMenu>
#include <iostream>
#include <QtWidgets>
#include <QDebug>
#include <QFont>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginWindow = new LoginWindow;
    signUpWindow = new SignUpWindow;
    dataWindow = new DataWindow;
    this->setCentralWidget(loginWindow);
    createActions();
    createToolBars(ENGLISH_LANGUAGE);
    mainToolBar->setVisible(false);
}

void MainWindow::createActions()
{
    settingButton = new QAction (QIcon(":/new/Icons/Resources/SettingsIcos.png"), tr(""), this);
    settingButton->setStatusTip(tr("Settings"));
    connect(settingButton, &QAction::triggered, this, &MainWindow::personalInfoSlot);

    danishAction = new QAction (QIcon(":/new/Icons/Resources/danishFlag.jpg"), tr("Danish"), this);
    danishAction->setStatusTip(tr("Danish"));
    connect(danishAction, &QAction::triggered, this, &MainWindow::danishSlot);

    englishAction = new QAction (QIcon(":/new/Icons/Resources/ukFlak.png"), tr("English"), this);
    englishAction->setStatusTip(tr("English"));
    connect(englishAction, &QAction::triggered, this, &MainWindow::englishSlot);

    themeLight = new QAction (tr("Light Theme"), this);
    themeLight->setStatusTip(tr("Select Light Theme"));
    connect(themeLight, &QAction::triggered, this, &MainWindow::lightThemeSlot);

    normalUnits = new QAction (tr("Kg / ºC"), this);
    normalUnits->setStatusTip(tr("Select SI Unit System"));
    connect(normalUnits, &QAction::triggered, this, &MainWindow::normalUnitSlot);

    americanUnits = new QAction (tr("Lbs / ºF"), this);
    americanUnits->setStatusTip(tr("Select American Unit System"));
    connect(normalUnits, &QAction::triggered, this, &MainWindow::americanUnitSlot);

    themeDark = new QAction (tr("Dark Theme"), this);
    themeDark->setStatusTip(tr("Select Dark Theme"));
    connect(themeDark, &QAction::triggered, this, &MainWindow::darkThemeSlot);

    logOut = new QAction (tr("Log Out"), this);
    logOut->setStatusTip(tr("Log Out"));
    connect(logOut, &QAction::triggered, this, &MainWindow::logOutSlot);

    homeButtonAction = new QAction (QIcon(":/new/Icons/Resources/homeButton.png"), tr(""), this);
    homeButtonAction->setStatusTip(tr("Home"));
    connect(homeButtonAction, &QAction::triggered, this, &MainWindow::homeButtonActionSlot);

    connect(loginWindow, &LoginWindow::correctUser, this, &MainWindow::accessDataSlot);
    connect(loginWindow, &LoginWindow::signUpSignal, this, &MainWindow::signUpSlot);
    connect(signUpWindow, &SignUpWindow::signUpAddUserSignal, this, &MainWindow::accessDataSlot);
    connect(signUpWindow, &SignUpWindow::singUpCancelSignUp, this, &MainWindow::logOutSlot);
}

void MainWindow::createToolBars(int language)
 {
    mainToolBar = addToolBar(tr("mainToolBar"));
    mainToolBar->setMovable(false);
    createMenu();

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    switch (language) {
    case ENGLISH_LANGUAGE:
        languageButton->setIcon(QIcon(":/new/Icons/Resources/ukFlak.png"));
        break;
    case DANISH_LANGUAGE:
        languageButton->setIcon(QIcon(":/new/Icons/Resources/danishFlag.jpg"));
        break;
    default:
        languageButton->setIcon(QIcon(":/new/Icons/Resources/ukFlak.png"));
        break;
    }

    mainToolBar->addAction(homeButtonAction);
    mainToolBar->addWidget(spacerWidget);
    mainToolBar->addWidget(unitsButton);
    mainToolBar->addWidget(themeButton);
    mainToolBar->addWidget(languageButton);
    mainToolBar->addWidget(userNameButton);
    mainToolBar->addAction(settingButton);
}

void MainWindow::createMenu()
{
    userNameMenu = new QMenu;
    userNameMenu->addAction(logOut);

    userNameButton = new QToolButton;
    QFont userNameButtonFont ("Courier", 20, QFont::Bold, true);
    userNameButton->setFont(userNameButtonFont);
    userNameButton->setMenu(userNameMenu);
    userNameButton->setText(tr("User Name"));
    userNameButton->setPopupMode(QToolButton::InstantPopup);

    languageMenu = new QMenu;
    languageMenu->addAction(danishAction);
    languageMenu->addAction(englishAction);

    languageButton = new QToolButton;
    languageButton->setMenu(languageMenu);
    languageButton->setPopupMode(QToolButton::InstantPopup);

    themeMenu = new QMenu;
    themeMenu->addAction(themeLight);
    themeMenu->addAction(themeDark);

    themeButton = new QToolButton;
    QFont themeButtonFont ("Courier", 16, QFont::StyleNormal, true);
    themeButton->setFont(themeButtonFont);
    themeButton->setMenu(themeMenu);
    themeButton->setText(tr("Theme"));
    themeButton->setPopupMode(QToolButton::InstantPopup);

    unitsMenu = new QMenu;
    unitsMenu->addAction(normalUnits);
    unitsMenu->addAction(americanUnits);

    unitsButton = new QToolButton;
    unitsButton->setFont(themeButtonFont);
    unitsButton->setMenu(unitsMenu);
    unitsButton->setText(tr("Units"));
    unitsButton->setPopupMode(QToolButton::InstantPopup);
}

void MainWindow::setNameWindowName()
{
    QString fullName;
    fullName = mainWindowUserDatabaseManager.userDataBaseRetrieveFullName(id);
    userNameButton->setText(fullName);
}

void MainWindow::updateWindowName(QString fullName){
    userNameButton->setText(fullName);
}

void MainWindow::englishSlot()
{
    languageButton->setIcon(QIcon(":/new/Icons/Resources/Ukflag.png"));
    mainToolBar->clear();
    createToolBars(ENGLISH_LANGUAGE);
    setNameWindowName();
}

void MainWindow::danishSlot()
{
    languageButton->setIcon(QIcon(":/new/Icons/Resources/danishFlag.jpg"));
    mainToolBar->clear();
    createToolBars(DANISH_LANGUAGE);
    setNameWindowName();
}

void MainWindow::lightThemeSlot()
{

}

void MainWindow::darkThemeSlot()
{

}

void MainWindow::normalUnitSlot()
{

}

void MainWindow::americanUnitSlot()
{

}

void MainWindow::personalInfoSlot()
{
    QString idString;
    qDebug() << "MainWindow::personalInfoSlot - ID: " << idString.number(id);
    optionsWindow = new OptionsWindow (this, id);
    connect(optionsWindow, &OptionsWindow::apply_changes, this, &MainWindow::updateWindowName);
    this->setCentralWidget(optionsWindow);
    mainToolBar->setVisible(true);
    this->setWindowTitle("Personal Information");
}

void MainWindow::logOutSlot()
{
    loginWindow = new LoginWindow;
    qDebug() << "MainWindow::logOutSlot - creating actions";
    connect(loginWindow, &LoginWindow::correctUser, this, &MainWindow::accessDataSlot);
    connect(loginWindow, &LoginWindow::signUpSignal, this, &MainWindow::signUpSlot);
    this->setCentralWidget(loginWindow);
    mainToolBar->setVisible(false);
}

void MainWindow::signUpSlot()
{
    signUpWindow = new SignUpWindow;
    createActions();
    this->setCentralWidget(signUpWindow);
    mainToolBar->setVisible(false);
    this->setWindowTitle("Sign Up");
}

void MainWindow::homeButtonActionSlot()
{
    dataWindow = new DataWindow;
    this->setCentralWidget(dataWindow);
    mainToolBar->setVisible(true);
    setNameWindowName();
    this->setWindowTitle("Personal Data");
    connect(dataWindow, &DataWindow::openExtraPickup, this, &MainWindow::extraPickUpSlot);
}

void MainWindow::accessDataSlot(int userId)
{
    id = userId;
    QString idString;
    qDebug() << "MainWindow::accessDataSlot - ID: " << idString.number(id);
    dataWindow = new DataWindow (this, id);
    this->setCentralWidget(dataWindow);
    mainToolBar->setVisible(true);
    setNameWindowName();
    this->setWindowTitle("Personal Data");
    connect(dataWindow, &DataWindow::openExtraPickup, this, &MainWindow::extraPickUpSlot);
}

void MainWindow::extraPickUpSlot(int id)
{
    extraPickupWindow = new ExtraPickupWindow (this, id);
    this->setCentralWidget(extraPickupWindow);
    mainToolBar->setVisible(true);
    setNameWindowName();
    this->setWindowTitle("Personal Data");
}

MainWindow::~MainWindow()
{
    delete ui;
}
