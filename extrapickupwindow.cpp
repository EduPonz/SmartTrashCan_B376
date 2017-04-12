#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent, int id) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);
}

ExtraPickupWindow::~ExtraPickupWindow()
{
    delete ui;
}
