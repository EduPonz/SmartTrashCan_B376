#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include "trashinfodatabasemanager.h"
#include "userdatabasemanager.h"


namespace Ui {
class DataWindow;
}
class DataWindow : public QWidget
{
    Q_OBJECT
public:
    DataWindow(QWidget *parent = 0, int id = 0);
    ~DataWindow();
    void readTrashData();

signals:
    void openExtraPickup(int id);

public slots:
    void readSerial();

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

    void on_temperatureTitleButton_clicked();

private:
    QSerialPort *arduino;

    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 66;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data_1;
    QString parsed_data_2;
    QString parsed_data_3;
    QString parsed_data_4;

    float todayFullness;
    float today_1Fullness;
    float today_2Fullness;
    float today_3Fullness;
    float today_4Fullness;
    float today_5Fullness;
    float today_6Fullness;

    float weekFullness;
    float week_1Fullness;
    float week_2Fullness;
    float week_3Fullness;
    float week_4Fullness;
    float week_5Fullness;
    float week_6Fullness;

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

    float todayWeight;
    float today_1Weight;
    float today_2Weight;
    float today_3Weight;
    float today_4Weight;
    float today_5Weight;
    float today_6Weight;

    float weekWeight;
    float week_1Weight;
    float week_2Weight;
    float week_3Weight;
    float week_4Weight;
    float week_5Weight;
    float week_6Weight;

    float monthWeight;
    float month_1Weight;
    float month_2Weight;
    float month_3Weight;
    float month_4Weight;
    float month_5Weight;
    float month_6Weight;
    float month_7Weight;
    float month_8Weight;
    float month_9Weight;
    float month_10Weight;
    float month_11Weight;

    float yearWeight;
    float year_1Weight;
    float year_2Weight;
    float year_3Weight;
    float year_4Weight;

    float todayHumidity;
    float today_1Humidity;
    float today_2Humidity;
    float today_3Humidity;
    float today_4Humidity;
    float today_5Humidity;
    float today_6Humidity;

    float weekHumidity;
    float week_1Humidity;
    float week_2Humidity;
    float week_3Humidity;
    float week_4Humidity;
    float week_5Humidity;
    float week_6Humidity;

    float monthHumidity;
    float month_1Humidity;
    float month_2Humidity;
    float month_3Humidity;
    float month_4Humidity;
    float month_5Humidity;
    float month_6Humidity;
    float month_7Humidity;
    float month_8Humidity;
    float month_9Humidity;
    float month_10Humidity;
    float month_11Humidity;

    float yearHumidity;
    float year_1Humidity;
    float year_2Humidity;
    float year_3Humidity;
    float year_4Humidity;

    float todayTemperature;
    float today_1Temperature;
    float today_2Temperature;
    float today_3Temperature;
    float today_4Temperature;
    float today_5Temperature;
    float today_6Temperature;

    float weekTemperature;
    float week_1Temperature;
    float week_2Temperature;
    float week_3Temperature;
    float week_4Temperature;
    float week_5Temperature;
    float week_6Temperature;

    float monthTemperature;
    float month_1Temperature;
    float month_2Temperature;
    float month_3Temperature;
    float month_4Temperature;
    float month_5Temperature;
    float month_6Temperature;
    float month_7Temperature;
    float month_8Temperature;
    float month_9Temperature;
    float month_10Temperature;
    float month_11Temperature;

    float yearTemperature;
    float year_1Temperature;
    float year_2Temperature;
    float year_3Temperature;
    float year_4Temperature;

    void createFullnessChart(int tab_index);
    void createWeightChart(int tab_index);
    void createHumidityChart(int tab_index);
    void createTemperatureChart(int tab_index);
    TrashInfoDatabaseManager trashInfoManager;
    UserDataBaseManager userManager;

    int typeOfDataSelected;

    Ui::DataWindow *ui;
    int userId;
    void dailyFullnessInit();
    void weeklyFullnessInit();
    void monthlyFullnessInit();
    void yearlyFullnessInit();

    void dailyWeightInit();
    void weeklyWeightInit();
    void monthlyWeightInit();
    void yearlyWeightInit();

    void dailyHumidityInit();
    void weeklyHumidityInit();
    void monthlyHumidityInit();
    void yearlyHumidityInit();

    void dailyTemperatureInit();
    void weeklyTemperatureInit();
    void monthlyTemperatureInit();
    void yearlyTemperatureInit();

    void currentDataShow();

};

#endif // DATAWINDOW_H
