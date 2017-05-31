/*! \class OptionsWindow
 *  \brief The user's personal information
 *  is shown in the OptionsWindow,
 *  accessible by clicking the tool
 *  icon on the top right of the page.
 *  From where the user can access and
 *  update his personal information or simply delete his profile.
 * */


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
