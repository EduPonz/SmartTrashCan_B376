#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QString>
#include "userdatabasemanager.h"
#include "historydatabasemanager.h"

namespace Ui {
class OptionsWindow;
}

class OptionsWindow : public QWidget
{
    Q_OBJECT

public:
    OptionsWindow(QWidget *parent = 0, int id = 0);
    ~OptionsWindow();

signals:
    void apply_changes(QString fullName);
    void user_deleted();

private slots:
    void on_optionsApplyChangesButton_clicked();
    void on_optionsDeleteUserButton_clicked();

private:
    Ui::OptionsWindow *ui;
    UserDataBaseManager optionsDatabaseManager;
    HistoryDatabaseManager historyManager;
    QSqlQuery optionsQuery;
    int userId;

    void populateTable();
    void populate();
};

#endif // OPTIONSWINDOW_H
