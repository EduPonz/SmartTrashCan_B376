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

    float todayFullness;
    float today_1Fullness;
    float today_2Fullness;
    float today_3Fullness;
    float today_4Fullness;
    float today_5Fullness;
    float today_6Fullness;

    float monthFullness;
    float month_1Fullness;
    float month_2Fullness;
    float month_3Fullness;
    float month_4Fullness;
    float month_5Fullness;
    float month_6Fullness;
    float month_7Fullness;
    float month_8Fullness;
    float month_9Fullness;
    float month_10Fullness;
    float month_11Fullness;

    float yearFullness;
    float year_1Fullness;
    float year_2Fullness;
    float year_3Fullness;
    float year_4Fullness;

    void createFullnessChart(int tab_index);
    void createWeightChart(int tab_index);
    void createHumidityChart(int tab_index);
    void createTemperatureChart(int tab_index);
    TrashInfoDatabaseManager trashInfoManager;

    int typeOfDataSelected;

    Ui::DataWindow *ui;
    int userId;
    void dailyFullnessInit();
    void weeklyFullnessInit();
    void monthlyFullnessInit();
    void yearlyFullnessInit();
};

#endif // DATAWINDOW_H
