/*! \class DataWindow class.
    \brief Controls the datawindow.ui. It displays current trash can data, next pick up date and a button to request extra pickup
    servies. It creates and displays charts regarding trash can usage, using the information stored in the trashInfo table in the database.
*/

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
    DataWindow(QWidget *parent = 0, int id = 0); /*!<Sets the UI to datawindow.ui. Creates objects of (...). Creates a sets the application Toolbar.*/
    ~DataWindow(); /*!<Deletes the datawindow.ui UI.*/
    void readTrashData(); /*!<Slot triggered when Arduino is connected. Check if Arduino is available and identify the port the arduino uno is on. Accesses the serial port and reads values for the 4 trash data from the connected Arduino. */

signals:
    void openExtraPickup(int id); /*!<Opens extra pickup window for the logged in user ID. */

public slots:
    void readSerial(); /*!<Slot triggered when Arduino is connected. Identifies the data coming from the Arduino through the Serial Port. Reads the serial port to obtain a Unicode, then convert the parsed data to floats and print theses values for each of the 4 data types received from the Arduino. */

private slots:
    void on_datawindowPickupButton_clicked(); /*!<Slot triggered when the user clicks Extra Pickup Services button. Emits a signal to open the extra pickup window using openExtraPickup(userId);. */
    void on_fullnessTitleButton_clicked(); /*!<Slot triggered when the user clicks on fullness title widget. Calls for on_tabWidget_currentChanged(0); for the fullness data to be shown in the chart widget.*/
    void on_fullnessDataButton_clicked(); /*!<Slot triggered when the user clicks on fullness data widget. Calls for on_tabWidget_currentChanged(0); for the fullness data to be shown in the chart widget.*/
    void on_weigthTitleButton_clicked(); /*!<Slot triggered when the user clicks on weight title widget. Calls for on_tabWidget_currentChanged(0); for the weigth data to be shown in the chart widget.*/
    void on_humidityTitleButton_clicked(); /*!<Slot triggered when the user clicks on humidity title widget. Calls for on_tabWidget_currentChanged(0); for the humidity data to be shown in the chart widget.*/
    void on_temperatureDataButton_clicked(); /*!<Slot triggered when the user clicks on temperature data widget. Calls for on_tabWidget_currentChanged(0); for the temperature data to be shown in the chart widget.*/
    void on_nextPickUpTitleButton_clicked(); /*!<Slot triggered when the user clicks on next pickup title widget but does not change the data chart shown .*/
    void on_weightDataButton_clicked(); /*!<Slot triggered when the user clicks on weight data widget. Calls for on_tabWidget_currentChanged(0); for the weight data to be shown in the chart widget.*/
    void on_humidityDataButton_clicked(); /*!<Slot triggered when the user clicks on humidity data widget. Calls for on_tabWidget_currentChanged(0); for the humidity data to be shown in the chart widget.*/
    void on_temperatutureTitleButton_clicked(); /*!<Slot triggered when the user clicks on temperature title widget. Calls for on_tabWidget_currentChanged(0); for the temperature data to be shown in the chart widget.*/
    void on_nextPickUpDataButton_clicked(); /*!<Slot triggered when the user clicks on next pickup data widget but does not change the data chart shown .*/

    void on_tabWidget_currentChanged(int index); /*!<Receives the case number which determines the data to be shown in the chart widget.*/

    void on_temperatureTitleButton_clicked(); /*!<Selects temperature title button and calls for on_tabWidget_currentChanged(0); for the temperatuture data to be shown in the chart widget.*/

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

    void createFullnessChart(int tab_index);  /*!<Creates fullness chart ready to receive data for that user;.  */
    void createWeightChart(int tab_index);  /*!<Creates weight chart to receive data for that user;.  */
    void createHumidityChart(int tab_index);  /*!<Creates humidity chart to receive data for that user;.  */
    void createTemperatureChart(int tab_index);  /*!<Creates temperature chart to receive data for that user;.  */
    TrashInfoDatabaseManager trashInfoManager;
    UserDataBaseManager userManager;

    int typeOfDataSelected;

    Ui::DataWindow *ui;
    int userId;
    void dailyFullnessInit(); /*!<Initializes the daily fullness chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);.   */
    void weeklyFullnessInit(); /*!<Initializes the weekly fullness chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveWeekly(userId);.   */
    void monthlyFullnessInit(); /*!<Initializes the monthly fullness chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);.   */
    void yearlyFullnessInit(); /*!<Initializes the yearly fullness chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveYearly(userId);.   */

    void dailyWeightInit(); /*!<Initializes the daily weight chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);.   */
    void weeklyWeightInit(); /*!<Initializes the weekly weight chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveWeekly(userId);.   */
    void monthlyWeightInit(); /*!<Initializes the monthly weight chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);.   */
    void yearlyWeightInit(); /*!<Initializes the yearly weight chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveYearly(userId);.   */

    void dailyHumidityInit(); /*!<Initializes the daily humidity chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);.   */
    void weeklyHumidityInit(); /*!<Initializes the weekly humidity chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveWeekly(userId);.   */
    void monthlyHumidityInit(); /*!<Initializes the monthly humidity chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);.   */
    void yearlyHumidityInit(); /*!<Initializes the yearly humidity chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveYearly(userId);.   */

    void dailyTemperatureInit();/*!<Initializes the daily temperature chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);.   */
    void weeklyTemperatureInit(); /*!<Initializes the weekly temperature chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveWeekly(userId);.   */
    void monthlyTemperatureInit(); /*!<Initializes the monthly temperature chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);.   */
    void yearlyTemperatureInit(); /*!<Initializes the yearly temperature chart for that user from the data collected in the database using trashInfoManager.trashInfoDatabaseRetrieveYearly(userId);.   */

    void currentDataShow(); /*!<Uses trashInfoManager.trashInfoDatabaseRetrieveDaily(userId) to retrieve the current trash data from the database for a specific user and print it to the data window;.  */

};

#endif // DATAWINDOW_H
