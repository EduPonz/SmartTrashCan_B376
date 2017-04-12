#ifndef EXTRAPICKUPWINDOW_H
#define EXTRAPICKUPWINDOW_H

#include <QWidget>
#include <QString>

namespace Ui {
class ExtraPickupWindow;
}

class ExtraPickupWindow : public QWidget
{
    Q_OBJECT

public:
    ExtraPickupWindow(QWidget *parent = 0, int id = 0);
    ~ExtraPickupWindow();

private slots:
    void on_extrapickupConfirmButton_clicked();

    void mailSent(QString);
private:

    QString EMAIL_USER_NAME;
    QString EMAIL_PASSWORD;
    QString EMAIL_SERVER;
    int EMAIL_PORT;
    QString EMAIL_SUBJECT;

    //Message to be sent variables
    Ui::ExtraPickupWindow *ui;
};

#endif // EXTRAPICKUPWINDOW_H
