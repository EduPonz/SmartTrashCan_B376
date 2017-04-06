#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionswindow.h"
#include "loginwindow.h"
#include "datawindow.h"
#include <QMainWindow>
#include <QMenu>
#include <QAbstractButton>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QToolBar *mainToolBar;
    Ui::MainWindow *ui;

private slots:
     void personalInfoSlot();
     void logOutSlot();
     void homeButtonActionSlot();
     void accessDataSlot();

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

    QIcon settingsIcon;

    QMenu *settingsMenu;
    QMenu *userNameMenu;

    QToolButton *settingButton;
    QToolButton *userNameButton;

    OptionsWindow *optionsWindow;
    DataWindow *dataWindow;
    LoginWindow *loginWindow;
};

#endif // MAINWINDOW_H
