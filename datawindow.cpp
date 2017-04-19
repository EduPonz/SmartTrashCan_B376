#include "datawindow.h"
#include "ui_datawindow.h"

DataWindow::DataWindow(QWidget *parent, int id) : QWidget(parent), ui(new Ui::DataWindow)
{
    userId = id;
    ui->setupUi(this);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::on_datawindowPickupButton_clicked()
{
    emit openExtraPickup(userId);
}

void DataWindow::on_fullnessTitleButton_clicked()
{

}

void DataWindow::on_weigthTitleButton_clicked()
{

}

void DataWindow::on_humidityTitleButton_clicked()
{

}

void DataWindow::on_nextPickUpTitleButton_clicked()
{

}

void DataWindow::on_temperatutureTitleButton_clicked()
{

}

void DataWindow::on_fullnessDataButton_clicked()
{
    on_fullnessTitleButton_clicked();
}

void DataWindow::on_weightDataButton_clicked()
{
    on_weigthTitleButton_clicked();
}

void DataWindow::on_humidityDataButton_clicked()
{
    on_humidityTitleButton_clicked();
}

void DataWindow::on_temperatureDataButton_clicked()
{
    on_temperatutureTitleButton_clicked();
}

void DataWindow::on_nextPickUpDataButton_clicked()
{
    on_nextPickUpTitleButton_clicked();
}
