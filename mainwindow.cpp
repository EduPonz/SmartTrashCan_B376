#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QToolBar>
#include <QMenu>
#include <iostream>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
   ui(new Ui::MainWindow)
{
    createActions();
    createToolBars();
    ui->setupUi(this);
}

void MainWindow::createActions()
{
    personalInfo = new QAction (tr("Personal Information"), this);
    personalInfo->setStatusTip(tr("Personal Information"));

    language = new QAction (tr("Language"), this);
    language->setStatusTip(tr("Language"));

    theme = new QAction (tr("Theme"), this);
    theme->setStatusTip(tr("Theme"));

    units = new QAction (tr("Units"), this);
    units->setStatusTip(tr("Units"));
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
    mainToolBar->addWidget(spacerWidget);

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
}

void MainWindow::settingsSlot()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
