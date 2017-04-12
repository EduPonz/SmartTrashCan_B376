#ifndef EXTRAPICKUPWINDOW_H
#define EXTRAPICKUPWINDOW_H

#include <QWidget>

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
    Ui::ExtraPickupWindow *ui;
};

#endif // EXTRAPICKUPWINDOW_H
