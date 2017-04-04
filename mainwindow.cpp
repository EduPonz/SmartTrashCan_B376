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
    settingsAction = new QAction(QIcon(":/new/Icons/Resources/SettingsIcos.png") ,tr(""), this);
    //settingsAction = new QAction(tr("settings"), this);
    settingsAction->setStatusTip(tr("settings"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(settingsSlot()));
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

    mainToolBar->addWidget(settingsMenu);
}

void MainWindow::createMenu()
{
    settingsMenu = new QMenu;
    settingsMenu->setIcon(settingsIcon);
    settingsMenu->addAction(settingsAction);
}

void MainWindow::settingsSlot()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
