#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QToolBar>
#include <QMenu>
#include <iostream>

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
    settingsAction = new QAction(QIcon(":/new/Icons/Resources/SettingsIcos.png"), tr("&New"), this);
    settingsAction->setShortcuts(QKeySequence::New);
    settingsAction->setStatusTip(tr("settings"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(settingsSlot()));
    std::cout << settingsAction->text().toStdString();
}

void MainWindow::createToolBars()
 {
    settingsIcon = QIcon(":/new/Icons/Resources/SettingsIcos.png");
    mainToolBar = addToolBar(tr("mainToolBar"));
    //mainToolBar->addAction(settingsIcon, tr("&settingsAction"));
    createMenu();
    mainToolBar->addWidget(settingsMenu);
}

void MainWindow::createMenu()
{
    //settingsMenu->addAction(settingsAction);
}

void MainWindow::settingsSlot()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
