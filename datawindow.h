#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QString>
#include "trashinfodatabasemanager.h"


namespace Ui {
class DataWindow;
}
class DataWindow : public QWidget
{
    Q_OBJECT
public:
    DataWindow(QWidget *parent = 0, int id = 0);
    ~DataWindow();

signals:
    void openExtraPickup(int id);

private slots:
    void on_datawindowPickupButton_clicked();
    void on_fullnessTitleButton_clicked();
    void on_fullnessDataButton_clicked();
    void on_weigthTitleButton_clicked();
    void on_humidityTitleButton_clicked();
    void on_temperatureDataButton_clicked();
    void on_nextPickUpTitleButton_clicked();
    void on_weightDataButton_clicked();
    void on_humidityDataButton_clicked();
    void on_temperatutureTitleButton_clicked();
    void on_nextPickUpDataButton_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    QString todayString;
    QString today_1String;
    QString today_2String;
    QString today_3String;
    QString today_4String;
    QString today_5String;
    QString today_6String;

    float todayFulness;
    float today_1Fulness;
    float today_2Fulness;
    float today_3Fulness;
    float today_4Fulness;
    float today_5Fulness;
    float today_6Fulness;

    void dateStringsInit();

    void createFullnessChart(int tab_index);
    void createWeightChart(int tab_index);
    void createHumidityChart(int tab_index);
    void createTemperatureChart(int tab_index);
    TrashInfoDatabaseManager trashInfoManager;

    int typeOfDataSelected;

    Ui::DataWindow *ui;
    int userId;
    void dailyFullnessInit();
};

#endif // DATAWINDOW_H
