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
    explicit ExtraPickupWindow(QWidget *parent = 0);
    ~ExtraPickupWindow();

private:
    Ui::ExtraPickupWindow *ui;
};

#endif // EXTRAPICKUPWINDOW_H
