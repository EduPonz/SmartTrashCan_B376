#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "optionswindow.h"
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

private slots:
     void personalInfoSlot();
     void homeButtonActionSlot();

private:
    void createActions();
    void createToolBars();
    void createMenu();

    QToolBar *mainToolBar;

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

    Ui::MainWindow *ui;
    OptionsWindow *optionsWindow;
};

#endif // MAINWINDOW_H
