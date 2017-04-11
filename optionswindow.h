#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QString>
#include "userdatabasemanager.h"

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

private slots:
    void on_optionsApplyChangesButton_clicked();

private:
    Ui::OptionsWindow *ui;
    UserDataBaseManager optionsDatabaseManager;
    QSqlQuery optionsQuery;
    int userId;


    void populate();
};

#endif // OPTIONSWINDOW_H
