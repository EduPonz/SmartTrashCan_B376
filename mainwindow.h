#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
     void settingsSlot();

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
    QIcon settingsIcon;
    QMenu *settingsMenu;
    QToolButton *settingButton;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
