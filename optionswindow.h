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
    OptionsWindow(QWidget *parent = 0, QString user = "");
    ~OptionsWindow();

private:
    Ui::OptionsWindow *ui;
    UserDataBaseManager optionsDatabaseManager;
    QSqlQuery optionsQuery;
    QString userName;


    void populate();
};

#endif // OPTIONSWINDOW_H
