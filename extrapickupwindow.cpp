#include "extrapickupwindow.h"
#include "ui_extrapickupwindow.h"

ExtraPickupWindow::ExtraPickupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtraPickupWindow)
{
    ui->setupUi(this);
}

ExtraPickupWindow::~ExtraPickupWindow()
{
    delete ui;
}
