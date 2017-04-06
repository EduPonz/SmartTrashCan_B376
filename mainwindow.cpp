#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_optionswindow.h"
#include "optionswindow.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QtGui>
#include <QToolBar>
#include <QMenu>
#include <iostream>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    createActions();
    createToolBars();
    ui->setupUi(this);
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

    homeButtonAction = new QAction (QIcon(":/new/Icons/Resources/homeButton.png"), tr(""), this);
    personalInfo->setStatusTip(tr("Home"));
    connect(homeButtonAction, &QAction::triggered, this, &MainWindow::homeButtonActionSlot);
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
    userNameButton->setMenu(userNameMenu);
    userNameButton->setText(tr("User Name"));
    userNameButton->setPopupMode(QToolButton::InstantPopup);

}

void MainWindow::personalInfoSlot()
{
    loginWindow = new LoginWindow;
    this->setCentralWidget(loginWindow);
}

void MainWindow::homeButtonActionSlot(){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
