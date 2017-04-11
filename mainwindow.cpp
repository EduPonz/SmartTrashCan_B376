#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_optionswindow.h"
#include "optionswindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "signupwindow.h"
#include "ui_signupwindow.h"
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
    this->setCentralWidget(loginWindow);
    createActions();
    createToolBars();
    mainToolBar->setVisible(false);
}

void MainWindow::createActions()
{
    personalInfo = new QAction (tr("Personal Information"), this);
    personalInfo->setStatusTip(tr("Personal Information"));
    connect(personalInfo, &QAction::triggered, this, &MainWindow::personalInfoSlot);

    language = new QAction (tr("Language"), this);
    language->setStatusTip(tr("Language"));

    theme = new QAction (tr("Theme"), this);
    theme->setStatusTip(tr("Theme"));

    units = new QAction (tr("Units"), this);
    units->setStatusTip(tr("Units"));

    home = new QAction (tr("Home"), this);
    home->setStatusTip(tr("Home"));

    logOut = new QAction (tr("Log Out"), this);
    logOut->setStatusTip(tr("Log Out"));
    connect(logOut, &QAction::triggered, this, &MainWindow::logOutSlot);

    homeButtonAction = new QAction (QIcon(":/new/Icons/Resources/homeButton.png"), tr(""), this);
    personalInfo->setStatusTip(tr("Home"));
    connect(homeButtonAction, &QAction::triggered, this, &MainWindow::homeButtonActionSlot);

    connect(loginWindow, &LoginWindow::correctUser, this, &MainWindow::accessDataSlot);
    connect(loginWindow, &LoginWindow::signUpSignal, this, &MainWindow::signUpSlot);
    connect(signUpWindow, &SignUpWindow::signUpAddUserSignal, this, &MainWindow::accessDataSlot);
    connect(signUpWindow, &SignUpWindow::singUpCancelSignUp, this, &MainWindow::logOutSlot);
}

void MainWindow::createToolBars()
 {
    settingsIcon = QIcon(":/new/Icons/Resources/SettingsIcos.png");
    mainToolBar = addToolBar(tr("mainToolBar"));
    mainToolBar->setMovable(false);
    createMenu();

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    mainToolBar->addAction(homeButtonAction);
    mainToolBar->addWidget(spacerWidget);
    mainToolBar->addWidget(userNameButton);
    mainToolBar->addWidget(settingButton);
}

void MainWindow::createMenu()
{
    settingsMenu = new QMenu;
    settingsMenu->addAction(personalInfo);
    settingsMenu->addAction(language);
    settingsMenu->addAction(theme);
    settingsMenu->addAction(units);

    settingButton = new QToolButton;
    settingButton->setMenu(settingsMenu);
    settingButton->setIcon(QIcon(":/new/Icons/Resources/SettingsIcos.png"));
    settingButton->setPopupMode(QToolButton::InstantPopup);

    userNameMenu = new QMenu;
    userNameMenu->addAction(home);
    userNameMenu->addAction(logOut);

    userNameButton = new QToolButton;
    QFont userNameButtonFont ("Courier", 20, QFont::Bold, true);
    userNameButton->setFont(userNameButtonFont);
    userNameButton->setMenu(userNameMenu);
    userNameButton->setText(tr("User Name"));
    userNameButton->setPopupMode(QToolButton::InstantPopup);
}

void MainWindow::setNameWindowName(){
    QString fullName;
    fullName = mainWindowUserDatabaseManager.userDataBaseRetrieveFullName(id);
    userNameButton->setText(fullName);
}

void MainWindow::updateWindowName(QString fullName){
    userNameButton->setText(fullName);
}

void MainWindow::personalInfoSlot()
{
    QString idString;
    qDebug() << "MainWindow::personalInfoSlot - ID: " << idString.number(id);
    optionsWindow = new OptionsWindow (this, id);
    connect(personalInfo, &QAction::triggered, this, &MainWindow::personalInfoSlot);
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
    QString fullName;
    setNameWindowName();
    this->setWindowTitle("Personal Data");
}

void MainWindow::accessDataSlot(int userId){
    id = userId;
    QString idString;
    qDebug() << "MainWindow::accessDataSlot - ID: " << idString.number(id);
    dataWindow = new DataWindow;
    this->setCentralWidget(dataWindow);
    mainToolBar->setVisible(true);
    setNameWindowName();
    this->setWindowTitle("Personal Data");
}

MainWindow::~MainWindow()
{
    delete ui;
}
