#include "datawindow.h"
#include "ui_datawindow.h"
#include "trashinfodatabasemanager.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>
#include <QSqlQuery>
#include <QString>
#include <QDate>
#include <QDateTime>

DataWindow::DataWindow(QWidget *parent, int id) : QWidget(parent), ui(new Ui::DataWindow)
{
    userId = id;
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    if (userId > 0){
        readTrashData();
        currentDataShow();
    }
    typeOfDataSelected = 0;
    createFullnessChart(0);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::currentDataShow()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.last();
    int fullnessInt = query.value(1).toInt();
    QString fullness = QString::number(fullnessInt) + " %";
    int weightInt = query.value(2).toInt();
    QString weight = QString::number(weightInt) + " Kg";
    int humidityInt = query.value(3).toInt();
    QString humidity = QString::number(humidityInt)+ " %";
    QString temperature = query.value(4).toString() + " ºC";

    ui->fullnessDataButton->setText(fullness);
    ui->weightDataButton->setText(weight);
    ui->humidityDataButton->setText(humidity);
    ui->temperatureDataButton->setText(temperature);

    int pickup_day = userManager.userDataBaseRetrievePickupDay(userId);
    int current_weekDay = QDateTime::currentDateTime().date().dayOfWeek();
    if (pickup_day <= current_weekDay){
        int days_to_advance = current_weekDay - pickup_day;
        QString date_to_show = QDate::fromString(QDate::currentDate().addDays(days_to_advance).
                                                     toString(Qt::ISODate), Qt::ISODate).toString();
        ui->nextPickUpDataButton->setText(date_to_show);
    }else{
        int days_to_advance = pickup_day - current_weekDay;
        QString date_to_show = QDate::fromString(QDate::currentDate().addDays(days_to_advance).
                                                     toString(Qt::ISODate), Qt::ISODate).toString();
        ui->nextPickUpDataButton->setText(date_to_show);
    }
}

void DataWindow::readTrashData()
{
    arduino = new QSerialPort(this);

    /*
    //  Testing code, prints the description, vendor id, and product id of all ports.
    //  Used it to determine the values for the arduino uno.

    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
    }
    */

    serialBuffer = "";
    parsed_data_1 = "";
    parsed_data_2 = "";
    parsed_data_3 = "";
    parsed_data_4 = "";

    //   Identify the port the arduino uno is on.

    bool arduino_is_available = false;
    QString arduino_uno_port_name;

    //  For each available serial port
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
    //  check if the serialport has both a product identifier and a vendor identifier
        if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier()){
        //  check if the product ID and the vendor ID match those of the arduino uno
            if((serialPortInfo.productIdentifier() == arduino_uno_product_id)
                    && (serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)){
                       arduino_is_available = true; //    arduino uno is available on this port
                       arduino_uno_port_name = serialPortInfo.portName();
             }
         }
    }


    //Open and configure the arduino port if available

    if(arduino_is_available){
        arduino->setPortName(arduino_uno_port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }else{
        qDebug() << "Couldn't find the correct port for the arduino.\n";
    }
}

void DataWindow::readSerial()
{
    /*
     * readyRead() doesn't guarantee that the entire message will be received all at once.
     * The message can arrive split into parts.  Need to buffer the serial data and then parse for the temperature value.
     *
     */
    QStringList buffer_split = serialBuffer.split(", "); //  split the serialBuffer string, parsing with ',' as the separator

    //  Check to see if there less than 3 tokens in buffer_split.
    //  If there are at least 3 then this means there were 2 commas,
    //  means there is a parsed temperature value as the second token (between 2 commas)
    if(buffer_split.length() < 6){
        // no parsed value yet so continue accumulating bytes from serial in the buffer.
        serialData = arduino->readAll();
        serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
        serialData.clear();
    }else{
        // the second element of buffer_split is parsed correctly, update the temperature value on temp_lcdNumber
        qDebug() << buffer_split << "\n";
        serialBuffer = "";
        if (buffer_split[0] == buffer_split[1]){
            serialBuffer = "";
            parsed_data_1 = buffer_split[1];
            parsed_data_2 = buffer_split[2];
            parsed_data_3 = buffer_split[3];
            parsed_data_4 = buffer_split[4];
            qDebug() << "Fullness: " << parsed_data_1;
            qDebug() << "Weight: " << parsed_data_2;
            qDebug() << "Humidity: " << parsed_data_3;
            qDebug() << "Temperature: " << parsed_data_4;
            float fullness = parsed_data_1.toFloat();
            float weigth = parsed_data_2.toFloat();
            float humidity = parsed_data_3.toFloat();
            float temperature = parsed_data_4.toFloat();
            trashInfoManager.trashInfoDatabaseInsert(userId, fullness, weigth, humidity, temperature);
        }
    }
}

void DataWindow::dailyFullnessInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.first();
    todayFullness = 0;
    today_1Fullness = 0;
    today_2Fullness = 0;
    today_3Fullness = 0;
    today_4Fullness = 0;
    today_5Fullness = 0;
    today_6Fullness = 0;
    int rowsFullness = 0;
    int rowsFullness_1 = 0;
    int rowsFullness_2 = 0;
    int rowsFullness_3 = 0;
    int rowsFullness_4 = 0;
    int rowsFullness_5 = 0;
    int rowsFullness_6 = 0;

    QString todayString = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_1String = QDateTime::fromString(QDate::currentDate().addDays(-1).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_2String = QDateTime::fromString(QDate::currentDate().addDays(-2).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_3String = QDateTime::fromString(QDate::currentDate().addDays(-3).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_4String = QDateTime::fromString(QDate::currentDate().addDays(-4).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_5String = QDateTime::fromString(QDate::currentDate().addDays(-5).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_6String = QDateTime::fromString(QDate::currentDate().addDays(-6).toString(Qt::ISODate), Qt::ISODate).date().toString();

    do {
        QString databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().toString();
        if (!databaseDate.compare(todayString))
        {
            todayFullness += query.value(1).toFloat();
            rowsFullness ++;
        }
        if (!databaseDate.compare(today_1String))
        {
            today_1Fullness += query.value(1).toFloat();
            rowsFullness_1 ++;
        }
        if (!databaseDate.compare(today_2String))
        {
            today_2Fullness += query.value(1).toFloat();
            rowsFullness_2 ++;
        }
        if (!databaseDate.compare(today_3String))
        {
            today_3Fullness += query.value(1).toFloat();
            rowsFullness_3 ++;
        }
        if (!databaseDate.compare(today_4String))
        {
            today_4Fullness += query.value(1).toFloat();
            rowsFullness_4 ++;
        }
        if (!databaseDate.compare(today_5String))
        {
            today_5Fullness += query.value(1).toFloat();
            rowsFullness_5 ++;
        }
        if (!databaseDate.compare(today_6String))
        {
            today_6Fullness += query.value(1).toFloat();
            rowsFullness_6 ++;
        }
    }while (query.next());

    if (rowsFullness != 0)
        todayFullness = todayFullness / rowsFullness;
    if (rowsFullness_1 !=0)
        today_1Fullness = today_1Fullness / rowsFullness_1;
    if (rowsFullness_2 !=0)
        today_2Fullness = today_2Fullness / rowsFullness_2;
    if (rowsFullness_3 !=0)
        today_3Fullness = today_3Fullness / rowsFullness_3;
    if (rowsFullness_4 !=0)
        today_4Fullness = today_4Fullness / rowsFullness_4;
    if (rowsFullness_5 !=0)
        today_5Fullness = today_5Fullness / rowsFullness_5;
    if (rowsFullness_6 !=0)
        today_6Fullness = today_6Fullness / rowsFullness_6;
}

void DataWindow::weeklyFullnessInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    weekFullness = 0;
    week_1Fullness = 0;
    week_2Fullness = 0;
    week_3Fullness = 0;
    week_4Fullness = 0;
    week_5Fullness = 0;
    week_6Fullness = 0;

    int rowsFullness = 0;
    int rowsFullness_1 = 0;
    int rowsFullness_2 = 0;
    int rowsFullness_3 = 0;
    int rowsFullness_4 = 0;
    int rowsFullness_5 = 0;
    int rowsFullness_6 = 0;

    int weekInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_1Int = QDateTime::fromString(QDate::currentDate().addDays(-7).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_2Int = QDateTime::fromString(QDate::currentDate().addDays(-14).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_3Int = QDateTime::fromString(QDate::currentDate().addDays(-21).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_4Int = QDateTime::fromString(QDate::currentDate().addDays(-28).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_5Int = QDateTime::fromString(QDate::currentDate().addDays(-35).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_6Int = QDateTime::fromString(QDate::currentDate().addDays(-42).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().weekNumber();
        if (databaseDate == weekInt)
        {
            weekFullness += query.value(1).toFloat();
            rowsFullness ++;
        }
        if (databaseDate == week_1Int)
        {
            week_1Fullness += query.value(1).toFloat();
            rowsFullness_1 ++;
        }
        if (databaseDate == week_2Int)
        {
            week_2Fullness += query.value(1).toFloat();
            rowsFullness_2 ++;
        }
        if (databaseDate == week_3Int)
        {
            week_3Fullness += query.value(1).toFloat();
            rowsFullness_3 ++;
        }
        if (databaseDate == week_4Int)
        {
            week_4Fullness += query.value(1).toFloat();
            rowsFullness_4 ++;
        }
        if (databaseDate == week_5Int)
        {
            week_5Fullness += query.value(1).toFloat();
            rowsFullness_5 ++;
        }
        if (databaseDate == week_6Int)
        {
            week_6Fullness += query.value(1).toFloat();
            rowsFullness_6 ++;
        }

    }while (query.next());

    if (rowsFullness != 0)
        weekFullness = weekFullness / rowsFullness;
    if (rowsFullness_1 !=0)
        week_1Fullness = week_1Fullness / rowsFullness_1;
    if (rowsFullness_2 !=0)
        week_2Fullness = week_2Fullness / rowsFullness_2;
    if (rowsFullness_3 !=0)
        week_3Fullness = week_3Fullness / rowsFullness_3;
    if (rowsFullness_4 !=0)
        week_4Fullness = week_4Fullness / rowsFullness_4;
    if (rowsFullness_5 !=0)
        week_5Fullness = week_5Fullness / rowsFullness_5;
    if (rowsFullness_6 !=0)
        week_6Fullness = week_6Fullness / rowsFullness_6;
}

void DataWindow::monthlyFullnessInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    monthFullness = 0;
    month_1Fullness = 0;
    month_2Fullness = 0;
    month_3Fullness = 0;
    month_4Fullness = 0;
    month_5Fullness = 0;
    month_6Fullness = 0;
    month_7Fullness = 0;
    month_8Fullness = 0;
    month_9Fullness = 0;
    month_10Fullness = 0;
    month_11Fullness = 0;
    int rowsFullness = 0;
    int rowsFullness_1 = 0;
    int rowsFullness_2 = 0;
    int rowsFullness_3 = 0;
    int rowsFullness_4 = 0;
    int rowsFullness_5 = 0;
    int rowsFullness_6 = 0;
    int rowsFullness_7 = 0;
    int rowsFullness_8 = 0;
    int rowsFullness_9 = 0;
    int rowsFullness_10 = 0;
    int rowsFullness_11 = 0;

    int monthInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_1Int = QDateTime::fromString(QDate::currentDate().addMonths(-1).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_2Int = QDateTime::fromString(QDate::currentDate().addMonths(-2).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_3Int = QDateTime::fromString(QDate::currentDate().addMonths(-3).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_4Int = QDateTime::fromString(QDate::currentDate().addMonths(-4).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_5Int = QDateTime::fromString(QDate::currentDate().addMonths(-5).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_6Int = QDateTime::fromString(QDate::currentDate().addMonths(-6).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_7Int = QDateTime::fromString(QDate::currentDate().addMonths(-7).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_8Int = QDateTime::fromString(QDate::currentDate().addMonths(-8).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_9Int = QDateTime::fromString(QDate::currentDate().addMonths(-9).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_10Int = QDateTime::fromString(QDate::currentDate().addMonths(-10).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_11Int = QDateTime::fromString(QDate::currentDate().addMonths(-11).toString(Qt::ISODate), Qt::ISODate).date().month();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().month();
        if (databaseDate == monthInt)
        {
            monthFullness += query.value(1).toFloat();
            rowsFullness ++;
        }
        if (databaseDate == month_1Int)
        {
            month_1Fullness += query.value(1).toFloat();
            rowsFullness_1 ++;
        }
        if (databaseDate == month_2Int)
        {
            month_2Fullness += query.value(1).toFloat();
            rowsFullness_2 ++;
        }
        if (databaseDate == month_3Int)
        {
            month_3Fullness += query.value(1).toFloat();
            rowsFullness_3 ++;
        }
        if (databaseDate == month_4Int)
        {
            month_4Fullness += query.value(1).toFloat();
            rowsFullness_4 ++;
        }
        if (databaseDate == month_5Int)
        {
            month_5Fullness += query.value(1).toFloat();
            rowsFullness_5 ++;
        }
        if (databaseDate == month_6Int)
        {
            month_6Fullness += query.value(1).toFloat();
            rowsFullness_6 ++;
        }
        if (databaseDate == month_7Int)
        {
            month_7Fullness += query.value(1).toFloat();
            rowsFullness_7 ++;
        }
        if (databaseDate == month_8Int)
        {
            month_8Fullness += query.value(1).toFloat();
            rowsFullness_8 ++;
        }
        if (databaseDate == month_9Int)
        {
            month_9Fullness += query.value(1).toFloat();
            rowsFullness_9 ++;
        }
        if (databaseDate == month_10Int)
        {
            month_10Fullness += query.value(1).toFloat();
            rowsFullness_10 ++;
        }
        if (databaseDate == month_11Int)
        {
            month_11Fullness += query.value(1).toFloat();
            rowsFullness_11 ++;
        }
    }while (query.next());

    if (rowsFullness != 0)
        monthFullness = monthFullness / rowsFullness;
    if (rowsFullness_1 !=0)
        month_1Fullness = month_1Fullness / rowsFullness_1;
    if (rowsFullness_2 !=0)
        month_2Fullness = month_2Fullness / rowsFullness_2;
    if (rowsFullness_3 !=0)
        month_3Fullness = month_3Fullness / rowsFullness_3;
    if (rowsFullness_4 !=0)
        month_4Fullness = month_4Fullness / rowsFullness_4;
    if (rowsFullness_5 !=0)
        month_5Fullness = month_5Fullness / rowsFullness_5;
    if (rowsFullness_6 !=0)
        month_6Fullness = month_6Fullness / rowsFullness_6;
    if (rowsFullness_7 !=0)
        month_7Fullness = month_7Fullness / rowsFullness_7;
    if (rowsFullness_8 !=0)
        month_8Fullness = month_8Fullness / rowsFullness_8;
    if (rowsFullness_9 !=0)
        month_9Fullness = month_9Fullness / rowsFullness_9;
    if (rowsFullness_10 !=0)
        month_10Fullness = month_10Fullness / rowsFullness_10;
    if (rowsFullness_11 !=0)
        month_11Fullness = month_11Fullness / rowsFullness_11;
}

void DataWindow::yearlyFullnessInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    yearFullness = 0;
    year_1Fullness = 0;
    year_2Fullness = 0;
    year_3Fullness = 0;
    year_4Fullness = 0;

    int rowsFullness = 0;
    int rowsFullness_1 = 0;
    int rowsFullness_2 = 0;
    int rowsFullness_3 = 0;
    int rowsFullness_4 = 0;

    int yearInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_1Int = QDateTime::fromString(QDate::currentDate().addYears(-1).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_2Int = QDateTime::fromString(QDate::currentDate().addYears(-2).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_3Int = QDateTime::fromString(QDate::currentDate().addYears(-3).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_4Int = QDateTime::fromString(QDate::currentDate().addYears(-4).toString(Qt::ISODate), Qt::ISODate).date().year();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().year();
        if (databaseDate == yearInt)
        {
            yearFullness += query.value(1).toFloat();
            rowsFullness ++;
        }
        if (databaseDate == year_1Int)
        {
            year_1Fullness += query.value(1).toFloat();
            rowsFullness_1 ++;
        }
        if (databaseDate == year_2Int)
        {
            year_2Fullness += query.value(1).toFloat();
            rowsFullness_2 ++;
        }
        if (databaseDate == year_3Int)
        {
            year_3Fullness += query.value(1).toFloat();
            rowsFullness_3 ++;
        }
        if (databaseDate == year_4Int)
        {
            year_4Fullness += query.value(1).toFloat();
            rowsFullness_4 ++;
        }
    }while (query.next());

    if (rowsFullness != 0)
        yearFullness = yearFullness / rowsFullness;
    if (rowsFullness_1 !=0)
        year_1Fullness = year_1Fullness / rowsFullness_1;
    if (rowsFullness_2 !=0)
        year_2Fullness = year_2Fullness / rowsFullness_2;
    if (rowsFullness_3 !=0)
        year_3Fullness = year_3Fullness / rowsFullness_3;
    if (rowsFullness_4 !=0)
        year_4Fullness = year_4Fullness / rowsFullness_4;
}

void DataWindow::dailyWeightInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.first();
    todayWeight = 0;
    today_1Weight = 0;
    today_2Weight = 0;
    today_3Weight = 0;
    today_4Weight = 0;
    today_5Weight = 0;
    today_6Weight = 0;
    int rowsWeight = 0;
    int rowsWeight_1 = 0;
    int rowsWeight_2 = 0;
    int rowsWeight_3 = 0;
    int rowsWeight_4 = 0;
    int rowsWeight_5 = 0;
    int rowsWeight_6 = 0;

    QString todayString = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_1String = QDateTime::fromString(QDate::currentDate().addDays(-1).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_2String = QDateTime::fromString(QDate::currentDate().addDays(-2).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_3String = QDateTime::fromString(QDate::currentDate().addDays(-3).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_4String = QDateTime::fromString(QDate::currentDate().addDays(-4).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_5String = QDateTime::fromString(QDate::currentDate().addDays(-5).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_6String = QDateTime::fromString(QDate::currentDate().addDays(-6).toString(Qt::ISODate), Qt::ISODate).date().toString();

    do {
        QString databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().toString();
        if (!databaseDate.compare(todayString))
        {
            todayWeight += query.value(2).toFloat();
            rowsWeight ++;
        }
        if (!databaseDate.compare(today_1String))
        {
            today_1Weight += query.value(2).toFloat();
            rowsWeight_1 ++;
        }
        if (!databaseDate.compare(today_2String))
        {
            today_2Weight += query.value(2).toFloat();
            rowsWeight_2 ++;
        }
        if (!databaseDate.compare(today_3String))
        {
            today_3Weight += query.value(2).toFloat();
            rowsWeight_3 ++;
        }
        if (!databaseDate.compare(today_4String))
        {
            today_4Weight += query.value(2).toFloat();
            rowsWeight_4 ++;
        }
        if (!databaseDate.compare(today_5String))
        {
            today_5Weight += query.value(2).toFloat();
            rowsWeight_5 ++;
        }
        if (!databaseDate.compare(today_6String))
        {
            today_6Weight += query.value(2).toFloat();
            rowsWeight_6 ++;
        }
    }while (query.next());
}

void DataWindow::weeklyWeightInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    weekWeight = 0;
    week_1Weight = 0;
    week_2Weight = 0;
    week_3Weight = 0;
    week_4Weight = 0;
    week_5Weight = 0;
    week_6Weight = 0;

    int rowsWeight = 0;
    int rowsWeight_1 = 0;
    int rowsWeight_2 = 0;
    int rowsWeight_3 = 0;
    int rowsWeight_4 = 0;
    int rowsWeight_5 = 0;
    int rowsWeight_6 = 0;

    int weekInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_1Int = QDateTime::fromString(QDate::currentDate().addDays(-7).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_2Int = QDateTime::fromString(QDate::currentDate().addDays(-14).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_3Int = QDateTime::fromString(QDate::currentDate().addDays(-21).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_4Int = QDateTime::fromString(QDate::currentDate().addDays(-28).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_5Int = QDateTime::fromString(QDate::currentDate().addDays(-35).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_6Int = QDateTime::fromString(QDate::currentDate().addDays(-42).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().weekNumber();
        if (databaseDate == weekInt)
        {
            weekWeight += query.value(2).toFloat();
            rowsWeight ++;
        }
        if (databaseDate == week_1Int)
        {
            week_1Weight += query.value(2).toFloat();
            rowsWeight_1 ++;
        }
        if (databaseDate == week_2Int)
        {
            week_2Weight += query.value(2).toFloat();
            rowsWeight_2 ++;
        }
        if (databaseDate == week_3Int)
        {
            week_3Weight += query.value(2).toFloat();
            rowsWeight_3 ++;
        }
        if (databaseDate == week_4Int)
        {
            week_4Weight += query.value(2).toFloat();
            rowsWeight_4 ++;
        }
        if (databaseDate == week_5Int)
        {
            week_5Weight += query.value(2).toFloat();
            rowsWeight_5 ++;
        }
        if (databaseDate == week_6Int)
        {
            week_6Weight += query.value(2).toFloat();
            rowsWeight_6 ++;
        }

    }while (query.next());
}

void DataWindow::monthlyWeightInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    monthWeight = 0;
    month_1Weight = 0;
    month_2Weight = 0;
    month_3Weight = 0;
    month_4Weight = 0;
    month_5Weight = 0;
    month_6Weight = 0;
    month_7Weight = 0;
    month_8Weight = 0;
    month_9Weight = 0;
    month_10Weight = 0;
    month_11Weight = 0;
    int rowsWeight = 0;
    int rowsWeight_1 = 0;
    int rowsWeight_2 = 0;
    int rowsWeight_3 = 0;
    int rowsWeight_4 = 0;
    int rowsWeight_5 = 0;
    int rowsWeight_6 = 0;
    int rowsWeight_7 = 0;
    int rowsWeight_8 = 0;
    int rowsWeight_9 = 0;
    int rowsWeight_10 = 0;
    int rowsWeight_11 = 0;

    int monthInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_1Int = QDateTime::fromString(QDate::currentDate().addMonths(-1).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_2Int = QDateTime::fromString(QDate::currentDate().addMonths(-2).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_3Int = QDateTime::fromString(QDate::currentDate().addMonths(-3).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_4Int = QDateTime::fromString(QDate::currentDate().addMonths(-4).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_5Int = QDateTime::fromString(QDate::currentDate().addMonths(-5).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_6Int = QDateTime::fromString(QDate::currentDate().addMonths(-6).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_7Int = QDateTime::fromString(QDate::currentDate().addMonths(-7).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_8Int = QDateTime::fromString(QDate::currentDate().addMonths(-8).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_9Int = QDateTime::fromString(QDate::currentDate().addMonths(-9).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_10Int = QDateTime::fromString(QDate::currentDate().addMonths(-10).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_11Int = QDateTime::fromString(QDate::currentDate().addMonths(-11).toString(Qt::ISODate), Qt::ISODate).date().month();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().month();
        if (databaseDate == monthInt)
        {
            monthWeight += query.value(2).toFloat();
            rowsWeight ++;
        }
        if (databaseDate == month_1Int)
        {
            month_1Weight += query.value(2).toFloat();
            rowsWeight_1 ++;
        }
        if (databaseDate == month_2Int)
        {
            month_2Weight += query.value(2).toFloat();
            rowsWeight_2 ++;
        }
        if (databaseDate == month_3Int)
        {
            month_3Weight += query.value(2).toFloat();
            rowsWeight_3 ++;
        }
        if (databaseDate == month_4Int)
        {
            month_4Weight += query.value(2).toFloat();
            rowsWeight_4 ++;
        }
        if (databaseDate == month_5Int)
        {
            month_5Weight += query.value(2).toFloat();
            rowsWeight_5 ++;
        }
        if (databaseDate == month_6Int)
        {
            month_6Weight += query.value(2).toFloat();
            rowsWeight_6 ++;
        }
        if (databaseDate == month_7Int)
        {
            month_7Weight += query.value(2).toFloat();
            rowsWeight_7 ++;
        }
        if (databaseDate == month_8Int)
        {
            month_8Weight += query.value(2).toFloat();
            rowsWeight_8 ++;
        }
        if (databaseDate == month_9Int)
        {
            month_9Weight += query.value(2).toFloat();
            rowsWeight_9 ++;
        }
        if (databaseDate == month_10Int)
        {
            month_10Weight += query.value(2).toFloat();
            rowsWeight_10 ++;
        }
        if (databaseDate == month_11Int)
        {
            month_11Weight += query.value(2).toFloat();
            rowsWeight_11 ++;
        }
    }while (query.next());

}

void DataWindow::yearlyWeightInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    yearWeight = 0;
    year_1Weight = 0;
    year_2Weight = 0;
    year_3Weight = 0;
    year_4Weight = 0;

    int rowsWeight = 0;
    int rowsWeight_1 = 0;
    int rowsWeight_2 = 0;
    int rowsWeight_3 = 0;
    int rowsWeight_4 = 0;

    int yearInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_1Int = QDateTime::fromString(QDate::currentDate().addYears(-1).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_2Int = QDateTime::fromString(QDate::currentDate().addYears(-2).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_3Int = QDateTime::fromString(QDate::currentDate().addYears(-3).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_4Int = QDateTime::fromString(QDate::currentDate().addYears(-4).toString(Qt::ISODate), Qt::ISODate).date().year();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().year();
        if (databaseDate == yearInt)
        {
            yearWeight += query.value(2).toFloat();
            rowsWeight ++;
        }
        if (databaseDate == year_1Int)
        {
            year_1Weight += query.value(2).toFloat();
            rowsWeight_1 ++;
        }
        if (databaseDate == year_2Int)
        {
            year_2Weight += query.value(2).toFloat();
            rowsWeight_2 ++;
        }
        if (databaseDate == year_3Int)
        {
            year_3Weight += query.value(2).toFloat();
            rowsWeight_3 ++;
        }
        if (databaseDate == year_4Int)
        {
            year_4Weight += query.value(2).toFloat();
            rowsWeight_4 ++;
        }
    }while (query.next());

}

void DataWindow::dailyHumidityInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.first();
    todayHumidity = 0;
    today_1Humidity = 0;
    today_2Humidity = 0;
    today_3Humidity = 0;
    today_4Humidity = 0;
    today_5Humidity = 0;
    today_6Humidity = 0;
    int rowsHumidity = 0;
    int rowsHumidity_1 = 0;
    int rowsHumidity_2 = 0;
    int rowsHumidity_3 = 0;
    int rowsHumidity_4 = 0;
    int rowsHumidity_5 = 0;
    int rowsHumidity_6 = 0;

    QString todayString = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_1String = QDateTime::fromString(QDate::currentDate().addDays(-1).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_2String = QDateTime::fromString(QDate::currentDate().addDays(-2).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_3String = QDateTime::fromString(QDate::currentDate().addDays(-3).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_4String = QDateTime::fromString(QDate::currentDate().addDays(-4).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_5String = QDateTime::fromString(QDate::currentDate().addDays(-5).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_6String = QDateTime::fromString(QDate::currentDate().addDays(-6).toString(Qt::ISODate), Qt::ISODate).date().toString();

    do {
        QString databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().toString();
        if (!databaseDate.compare(todayString))
        {
            todayHumidity += query.value(3).toFloat();
            rowsHumidity ++;
        }
        if (!databaseDate.compare(today_1String))
        {
            today_1Humidity += query.value(3).toFloat();
            rowsHumidity_1 ++;
        }
        if (!databaseDate.compare(today_2String))
        {
            today_2Humidity += query.value(3).toFloat();
            rowsHumidity_2 ++;
        }
        if (!databaseDate.compare(today_3String))
        {
            today_3Humidity += query.value(3).toFloat();
            rowsHumidity_3 ++;
        }
        if (!databaseDate.compare(today_4String))
        {
            today_4Humidity += query.value(3).toFloat();
            rowsHumidity_4 ++;
        }
        if (!databaseDate.compare(today_5String))
        {
            today_5Humidity += query.value(3).toFloat();
            rowsHumidity_5 ++;
        }
        if (!databaseDate.compare(today_6String))
        {
            today_6Humidity += query.value(3).toFloat();
            rowsHumidity_6 ++;
        }
    }while (query.next());

    if (rowsHumidity != 0)
        todayHumidity = todayHumidity / rowsHumidity;
    if (rowsHumidity_1 !=0)
        today_1Humidity = today_1Humidity / rowsHumidity_1;
    if (rowsHumidity_2 !=0)
        today_2Humidity = today_2Humidity / rowsHumidity_2;
    if (rowsHumidity_3 !=0)
        today_3Humidity = today_3Humidity / rowsHumidity_3;
    if (rowsHumidity_4 !=0)
        today_4Humidity= today_4Humidity / rowsHumidity_4;
    if (rowsHumidity_5 !=0)
        today_5Humidity = today_5Humidity / rowsHumidity_5;
    if (rowsHumidity_6 !=0)
        today_6Humidity = today_6Humidity / rowsHumidity_6;
}

void DataWindow::weeklyHumidityInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    weekHumidity = 0;
    week_1Humidity = 0;
    week_2Humidity = 0;
    week_3Humidity = 0;
    week_4Humidity = 0;
    week_5Humidity = 0;
    week_6Humidity = 0;

    int rowsHumidity = 0;
    int rowsHumidity_1 = 0;
    int rowsHumidity_2 = 0;
    int rowsHumidity_3 = 0;
    int rowsHumidity_4 = 0;
    int rowsHumidity_5 = 0;
    int rowsHumidity_6 = 0;

    int weekInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_1Int = QDateTime::fromString(QDate::currentDate().addDays(-7).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_2Int = QDateTime::fromString(QDate::currentDate().addDays(-14).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_3Int = QDateTime::fromString(QDate::currentDate().addDays(-21).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_4Int = QDateTime::fromString(QDate::currentDate().addDays(-28).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_5Int = QDateTime::fromString(QDate::currentDate().addDays(-35).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_6Int = QDateTime::fromString(QDate::currentDate().addDays(-42).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().weekNumber();
        if (databaseDate == weekInt)
        {
            weekHumidity += query.value(3).toFloat();
            rowsHumidity ++;
        }
        if (databaseDate == week_1Int)
        {
            week_1Humidity += query.value(3).toFloat();
            rowsHumidity_1 ++;
        }
        if (databaseDate == week_2Int)
        {
            week_2Humidity += query.value(3).toFloat();
            rowsHumidity_2 ++;
        }
        if (databaseDate == week_3Int)
        {
            week_3Humidity += query.value(3).toFloat();
            rowsHumidity_3 ++;
        }
        if (databaseDate == week_4Int)
        {
            week_4Humidity += query.value(3).toFloat();
            rowsHumidity_4 ++;
        }
        if (databaseDate == week_5Int)
        {
            week_5Humidity += query.value(3).toFloat();
            rowsHumidity_5 ++;
        }
        if (databaseDate == week_6Int)
        {
            week_6Humidity += query.value(3).toFloat();
            rowsHumidity_6 ++;
        }

    }while (query.next());

    if (rowsHumidity != 0)
        weekHumidity = weekHumidity / rowsHumidity;
    if (rowsHumidity_1 !=0)
        week_1Humidity = week_1Humidity / rowsHumidity_1;
    if (rowsHumidity_2 !=0)
        week_2Humidity = week_2Humidity / rowsHumidity_2;
    if (rowsHumidity_3 !=0)
        week_3Humidity = week_3Humidity / rowsHumidity_3;
    if (rowsHumidity_4 !=0)
        week_4Humidity = week_4Humidity / rowsHumidity_4;
    if (rowsHumidity_5 !=0)
        week_5Humidity = week_5Humidity / rowsHumidity_5;
    if (rowsHumidity_6 !=0)
        week_6Humidity = week_6Humidity / rowsHumidity_6;
}

void DataWindow::monthlyHumidityInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    monthHumidity = 0;
    month_1Humidity = 0;
    month_2Humidity = 0;
    month_3Humidity = 0;
    month_4Humidity = 0;
    month_5Humidity = 0;
    month_6Humidity = 0;
    month_7Humidity = 0;
    month_8Humidity = 0;
    month_9Humidity = 0;
    month_10Humidity = 0;
    month_11Humidity = 0;
    int rowsHumidity = 0;
    int rowsHumidity_1 = 0;
    int rowsHumidity_2 = 0;
    int rowsHumidity_3 = 0;
    int rowsHumidity_4 = 0;
    int rowsHumidity_5 = 0;
    int rowsHumidity_6 = 0;
    int rowsHumidity_7 = 0;
    int rowsHumidity_8 = 0;
    int rowsHumidity_9 = 0;
    int rowsHumidity_10 = 0;
    int rowsHumidity_11 = 0;

    int monthInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_1Int = QDateTime::fromString(QDate::currentDate().addMonths(-1).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_2Int = QDateTime::fromString(QDate::currentDate().addMonths(-2).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_3Int = QDateTime::fromString(QDate::currentDate().addMonths(-3).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_4Int = QDateTime::fromString(QDate::currentDate().addMonths(-4).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_5Int = QDateTime::fromString(QDate::currentDate().addMonths(-5).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_6Int = QDateTime::fromString(QDate::currentDate().addMonths(-6).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_7Int = QDateTime::fromString(QDate::currentDate().addMonths(-7).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_8Int = QDateTime::fromString(QDate::currentDate().addMonths(-8).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_9Int = QDateTime::fromString(QDate::currentDate().addMonths(-9).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_10Int = QDateTime::fromString(QDate::currentDate().addMonths(-10).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_11Int = QDateTime::fromString(QDate::currentDate().addMonths(-11).toString(Qt::ISODate), Qt::ISODate).date().month();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().month();
        if (databaseDate == monthInt)
        {
            monthHumidity += query.value(3).toFloat();
            rowsHumidity ++;
        }
        if (databaseDate == month_1Int)
        {
            month_1Humidity += query.value(3).toFloat();
            rowsHumidity_1 ++;
        }
        if (databaseDate == month_2Int)
        {
            month_2Humidity += query.value(3).toFloat();
            rowsHumidity_2 ++;
        }
        if (databaseDate == month_3Int)
        {
            month_3Humidity += query.value(3).toFloat();
            rowsHumidity_3 ++;
        }
        if (databaseDate == month_4Int)
        {
            month_4Humidity += query.value(3).toFloat();
            rowsHumidity_4 ++;
        }
        if (databaseDate == month_5Int)
        {
            month_5Humidity += query.value(3).toFloat();
            rowsHumidity_5 ++;
        }
        if (databaseDate == month_6Int)
        {
            month_6Humidity += query.value(3).toFloat();
            rowsHumidity_6 ++;
        }
        if (databaseDate == month_7Int)
        {
            month_7Humidity += query.value(3).toFloat();
            rowsHumidity_7 ++;
        }
        if (databaseDate == month_8Int)
        {
            month_8Humidity += query.value(3).toFloat();
            rowsHumidity_8 ++;
        }
        if (databaseDate == month_9Int)
        {
            month_9Humidity += query.value(3).toFloat();
            rowsHumidity_9 ++;
        }
        if (databaseDate == month_10Int)
        {
            month_10Humidity += query.value(3).toFloat();
            rowsHumidity_10 ++;
        }
        if (databaseDate == month_11Int)
        {
            month_11Humidity += query.value(3).toFloat();
            rowsHumidity_11 ++;
        }
    }while (query.next());

    if (rowsHumidity != 0)
        monthHumidity = monthHumidity / rowsHumidity;
    if (rowsHumidity_1 !=0)
        month_1Humidity = month_1Humidity / rowsHumidity_1;
    if (rowsHumidity_2 !=0)
        month_2Humidity = month_2Humidity / rowsHumidity_2;
    if (rowsHumidity_3 !=0)
        month_3Humidity = month_3Humidity / rowsHumidity_3;
    if (rowsHumidity_4 !=0)
        month_4Humidity = month_4Humidity / rowsHumidity_4;
    if (rowsHumidity_5 !=0)
        month_5Humidity = month_5Humidity / rowsHumidity_5;
    if (rowsHumidity_6 !=0)
        month_6Humidity = month_6Humidity / rowsHumidity_6;
    if (rowsHumidity_7 !=0)
        month_7Humidity = month_7Humidity / rowsHumidity_7;
    if (rowsHumidity_8 !=0)
        month_8Humidity = month_8Humidity / rowsHumidity_8;
    if (rowsHumidity_9 !=0)
        month_9Humidity = month_9Humidity / rowsHumidity_9;
    if (rowsHumidity_10 !=0)
        month_10Humidity = month_10Humidity / rowsHumidity_10;
    if (rowsHumidity_11 !=0)
        month_11Humidity = month_11Humidity / rowsHumidity_11;
}

void DataWindow::yearlyHumidityInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    yearHumidity = 0;
    year_1Humidity = 0;
    year_2Humidity = 0;
    year_3Humidity = 0;
    year_4Humidity = 0;

    int rowsHumidity = 0;
    int rowsHumidity_1 = 0;
    int rowsHumidity_2 = 0;
    int rowsHumidity_3 = 0;
    int rowsHumidity_4 = 0;

    int yearInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_1Int = QDateTime::fromString(QDate::currentDate().addYears(-1).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_2Int = QDateTime::fromString(QDate::currentDate().addYears(-2).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_3Int = QDateTime::fromString(QDate::currentDate().addYears(-3).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_4Int = QDateTime::fromString(QDate::currentDate().addYears(-4).toString(Qt::ISODate), Qt::ISODate).date().year();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().year();
        if (databaseDate == yearInt)
        {
            yearHumidity += query.value(3).toFloat();
            rowsHumidity ++;
        }
        if (databaseDate == year_1Int)
        {
            year_1Humidity += query.value(3).toFloat();
            rowsHumidity_1 ++;
        }
        if (databaseDate == year_2Int)
        {
            year_2Humidity += query.value(3).toFloat();
            rowsHumidity_2 ++;
        }
        if (databaseDate == year_3Int)
        {
            year_3Humidity += query.value(3).toFloat();
            rowsHumidity_3 ++;
        }
        if (databaseDate == year_4Int)
        {
            year_4Humidity += query.value(3).toFloat();
            rowsHumidity_4 ++;
        }
    }while (query.next());

    if (rowsHumidity != 0)
        yearHumidity = yearHumidity / rowsHumidity;
    if (rowsHumidity_1 !=0)
        year_1Humidity = year_1Humidity / rowsHumidity_1;
    if (rowsHumidity_2 !=0)
        year_2Humidity = year_2Humidity / rowsHumidity_2;
    if (rowsHumidity_3 !=0)
        year_3Humidity = year_3Humidity / rowsHumidity_3;
    if (rowsHumidity_4 !=0)
        year_4Humidity = year_4Humidity / rowsHumidity_4;
}

void DataWindow::dailyTemperatureInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.first();
    todayTemperature = 0;
    today_1Temperature = 0;
    today_2Temperature = 0;
    today_3Temperature = 0;
    today_4Temperature = 0;
    today_5Temperature = 0;
    today_6Temperature = 0;
    int rowsTemperature = 0;
    int rowsTemperature_1 = 0;
    int rowsTemperature_2 = 0;
    int rowsTemperature_3 = 0;
    int rowsTemperature_4 = 0;
    int rowsTemperature_5 = 0;
    int rowsTemperature_6 = 0;

    QString todayString = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_1String = QDateTime::fromString(QDate::currentDate().addDays(-1).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_2String = QDateTime::fromString(QDate::currentDate().addDays(-2).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_3String = QDateTime::fromString(QDate::currentDate().addDays(-3).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_4String = QDateTime::fromString(QDate::currentDate().addDays(-4).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_5String = QDateTime::fromString(QDate::currentDate().addDays(-5).toString(Qt::ISODate), Qt::ISODate).date().toString();
    QString today_6String = QDateTime::fromString(QDate::currentDate().addDays(-6).toString(Qt::ISODate), Qt::ISODate).date().toString();

    do {
        QString databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().toString();
        if (!databaseDate.compare(todayString))
        {
            todayTemperature += query.value(4).toFloat();
            rowsTemperature ++;
        }
        if (!databaseDate.compare(today_1String))
        {
            today_1Temperature += query.value(4).toFloat();
            rowsTemperature_1 ++;
        }
        if (!databaseDate.compare(today_2String))
        {
            today_2Temperature += query.value(4).toFloat();
            rowsTemperature_2 ++;
        }
        if (!databaseDate.compare(today_3String))
        {
            today_3Temperature += query.value(4).toFloat();
            rowsTemperature_3 ++;
        }
        if (!databaseDate.compare(today_4String))
        {
            today_4Temperature += query.value(4).toFloat();
            rowsTemperature_4 ++;
        }
        if (!databaseDate.compare(today_5String))
        {
            today_5Temperature += query.value(4).toFloat();
            rowsTemperature_5 ++;
        }
        if (!databaseDate.compare(today_6String))
        {
            today_6Temperature += query.value(4).toFloat();
            rowsTemperature_6 ++;
        }
    }while (query.next());

    if (rowsTemperature != 0)
        todayTemperature = todayTemperature / rowsTemperature;
    if (rowsTemperature_1 !=0)
        today_1Temperature = today_1Temperature / rowsTemperature_1;
    if (rowsTemperature_2 !=0)
        today_2Temperature = today_2Temperature / rowsTemperature_2;
    if (rowsTemperature_3 !=0)
        today_3Temperature = today_3Temperature / rowsTemperature_3;
    if (rowsTemperature_4 !=0)
        today_4Temperature= today_4Temperature / rowsTemperature_4;
    if (rowsTemperature_5 !=0)
        today_5Temperature = today_5Temperature / rowsTemperature_5;
    if (rowsTemperature_6 !=0)
        today_6Temperature = today_6Temperature / rowsTemperature_6;
}

void DataWindow::weeklyTemperatureInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    weekTemperature = 0;
    week_1Temperature = 0;
    week_2Temperature = 0;
    week_3Temperature = 0;
    week_4Temperature = 0;
    week_5Temperature = 0;
    week_6Temperature = 0;

    int rowsTemperature = 0;
    int rowsTemperature_1 = 0;
    int rowsTemperature_2 = 0;
    int rowsTemperature_3 = 0;
    int rowsTemperature_4 = 0;
    int rowsTemperature_5 = 0;
    int rowsTemperature_6 = 0;

    int weekInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_1Int = QDateTime::fromString(QDate::currentDate().addDays(-7).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_2Int = QDateTime::fromString(QDate::currentDate().addDays(-14).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_3Int = QDateTime::fromString(QDate::currentDate().addDays(-21).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_4Int = QDateTime::fromString(QDate::currentDate().addDays(-28).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_5Int = QDateTime::fromString(QDate::currentDate().addDays(-35).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();
    int week_6Int = QDateTime::fromString(QDate::currentDate().addDays(-42).toString(Qt::ISODate), Qt::ISODate).date().weekNumber();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().weekNumber();
        if (databaseDate == weekInt)
        {
            weekTemperature += query.value(4).toFloat();
            rowsTemperature ++;
        }
        if (databaseDate == week_1Int)
        {
            week_1Temperature += query.value(4).toFloat();
            rowsTemperature_1 ++;
        }
        if (databaseDate == week_2Int)
        {
            week_2Temperature += query.value(4).toFloat();
            rowsTemperature_2 ++;
        }
        if (databaseDate == week_3Int)
        {
            week_3Temperature += query.value(4).toFloat();
            rowsTemperature_3 ++;
        }
        if (databaseDate == week_4Int)
        {
            week_4Temperature += query.value(4).toFloat();
            rowsTemperature_4 ++;
        }
        if (databaseDate == week_5Int)
        {
            week_5Temperature += query.value(4).toFloat();
            rowsTemperature_5 ++;
        }
        if (databaseDate == week_6Int)
        {
            week_6Temperature += query.value(4).toFloat();
            rowsTemperature_6 ++;
        }

    }while (query.next());

    if (rowsTemperature != 0)
        weekTemperature = weekTemperature / rowsTemperature;
    if (rowsTemperature_1 !=0)
        week_1Temperature = week_1Temperature / rowsTemperature_1;
    if (rowsTemperature_2 !=0)
        week_2Temperature = week_2Temperature / rowsTemperature_2;
    if (rowsTemperature_3 !=0)
        week_3Temperature = week_3Temperature / rowsTemperature_3;
    if (rowsTemperature_4 !=0)
        week_4Temperature = week_4Temperature / rowsTemperature_4;
    if (rowsTemperature_5 !=0)
        week_5Temperature = week_5Temperature / rowsTemperature_5;
    if (rowsTemperature_6 !=0)
        week_6Temperature = week_6Temperature / rowsTemperature_6;
}

void DataWindow::monthlyTemperatureInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    monthTemperature = 0;
    month_1Temperature = 0;
    month_2Temperature = 0;
    month_3Temperature = 0;
    month_4Temperature = 0;
    month_5Temperature = 0;
    month_6Temperature = 0;
    month_7Temperature = 0;
    month_8Temperature = 0;
    month_9Temperature = 0;
    month_10Temperature = 0;
    month_11Temperature = 0;
    int rowsTemperature = 0;
    int rowsTemperature_1 = 0;
    int rowsTemperature_2 = 0;
    int rowsTemperature_3 = 0;
    int rowsTemperature_4 = 0;
    int rowsTemperature_5 = 0;
    int rowsTemperature_6 = 0;
    int rowsTemperature_7 = 0;
    int rowsTemperature_8 = 0;
    int rowsTemperature_9 = 0;
    int rowsTemperature_10 = 0;
    int rowsTemperature_11 = 0;

    int monthInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_1Int = QDateTime::fromString(QDate::currentDate().addMonths(-1).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_2Int = QDateTime::fromString(QDate::currentDate().addMonths(-2).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_3Int = QDateTime::fromString(QDate::currentDate().addMonths(-3).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_4Int = QDateTime::fromString(QDate::currentDate().addMonths(-4).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_5Int = QDateTime::fromString(QDate::currentDate().addMonths(-5).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_6Int = QDateTime::fromString(QDate::currentDate().addMonths(-6).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_7Int = QDateTime::fromString(QDate::currentDate().addMonths(-7).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_8Int = QDateTime::fromString(QDate::currentDate().addMonths(-8).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_9Int = QDateTime::fromString(QDate::currentDate().addMonths(-9).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_10Int = QDateTime::fromString(QDate::currentDate().addMonths(-10).toString(Qt::ISODate), Qt::ISODate).date().month();
    int month_11Int = QDateTime::fromString(QDate::currentDate().addMonths(-11).toString(Qt::ISODate), Qt::ISODate).date().month();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().month();
        if (databaseDate == monthInt)
        {
            monthTemperature += query.value(4).toFloat();
            rowsTemperature ++;
        }
        if (databaseDate == month_1Int)
        {
            month_1Temperature += query.value(4).toFloat();
            rowsTemperature_1 ++;
        }
        if (databaseDate == month_2Int)
        {
            month_2Temperature += query.value(4).toFloat();
            rowsTemperature_2 ++;
        }
        if (databaseDate == month_3Int)
        {
            month_3Temperature += query.value(4).toFloat();
            rowsTemperature_3 ++;
        }
        if (databaseDate == month_4Int)
        {
            month_4Temperature += query.value(4).toFloat();
            rowsTemperature_4 ++;
        }
        if (databaseDate == month_5Int)
        {
            month_5Temperature += query.value(4).toFloat();
            rowsTemperature_5 ++;
        }
        if (databaseDate == month_6Int)
        {
            month_6Temperature += query.value(4).toFloat();
            rowsTemperature_6 ++;
        }
        if (databaseDate == month_7Int)
        {
            month_7Temperature += query.value(4).toFloat();
            rowsTemperature_7 ++;
        }
        if (databaseDate == month_8Int)
        {
            month_8Temperature += query.value(4).toFloat();
            rowsTemperature_8 ++;
        }
        if (databaseDate == month_9Int)
        {
            month_9Temperature += query.value(4).toFloat();
            rowsTemperature_9 ++;
        }
        if (databaseDate == month_10Int)
        {
            month_10Temperature += query.value(4).toFloat();
            rowsTemperature_10 ++;
        }
        if (databaseDate == month_11Int)
        {
            month_11Temperature += query.value(4).toFloat();
            rowsTemperature_11 ++;
        }
    }while (query.next());

    if (rowsTemperature != 0)
        monthTemperature = monthTemperature / rowsTemperature;
    if (rowsTemperature_1 !=0)
        month_1Temperature = month_1Temperature / rowsTemperature_1;
    if (rowsTemperature_2 !=0)
        month_2Temperature = month_2Temperature / rowsTemperature_2;
    if (rowsTemperature_3 !=0)
        month_3Temperature = month_3Temperature / rowsTemperature_3;
    if (rowsTemperature_4 !=0)
        month_4Temperature = month_4Temperature / rowsTemperature_4;
    if (rowsTemperature_5 !=0)
        month_5Temperature = month_5Temperature / rowsTemperature_5;
    if (rowsTemperature_6 !=0)
        month_6Temperature = month_6Temperature / rowsTemperature_6;
    if (rowsTemperature_7 !=0)
        month_7Temperature = month_7Temperature / rowsTemperature_7;
    if (rowsTemperature_8 !=0)
        month_8Temperature = month_8Temperature / rowsTemperature_8;
    if (rowsTemperature_9 !=0)
        month_9Temperature = month_9Temperature / rowsTemperature_9;
    if (rowsTemperature_10 !=0)
        month_10Temperature = month_10Temperature / rowsTemperature_10;
    if (rowsTemperature_11 !=0)
        month_11Temperature = month_11Temperature / rowsTemperature_11;
}

void DataWindow::yearlyTemperatureInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveMonthly(userId);
    query.first();

    yearTemperature = 0;
    year_1Temperature = 0;
    year_2Temperature = 0;
    year_3Temperature = 0;
    year_4Temperature = 0;

    int rowsTemperature = 0;
    int rowsTemperature_1 = 0;
    int rowsTemperature_2 = 0;
    int rowsTemperature_3 = 0;
    int rowsTemperature_4 = 0;

    int yearInt = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_1Int = QDateTime::fromString(QDate::currentDate().addYears(-1).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_2Int = QDateTime::fromString(QDate::currentDate().addYears(-2).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_3Int = QDateTime::fromString(QDate::currentDate().addYears(-3).toString(Qt::ISODate), Qt::ISODate).date().year();
    int year_4Int = QDateTime::fromString(QDate::currentDate().addYears(-4).toString(Qt::ISODate), Qt::ISODate).date().year();

    do {
        int databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().year();
        if (databaseDate == yearInt)
        {
            yearTemperature += query.value(4).toFloat();
            rowsTemperature ++;
        }
        if (databaseDate == year_1Int)
        {
            year_1Temperature += query.value(4).toFloat();
            rowsTemperature_1 ++;
        }
        if (databaseDate == year_2Int)
        {
            year_2Temperature += query.value(4).toFloat();
            rowsTemperature_2 ++;
        }
        if (databaseDate == year_3Int)
        {
            year_3Temperature += query.value(4).toFloat();
            rowsTemperature_3 ++;
        }
        if (databaseDate == year_4Int)
        {
            year_4Temperature += query.value(4).toFloat();
            rowsTemperature_4 ++;
        }
    }while (query.next());

    if (rowsTemperature != 0)
        yearTemperature = yearTemperature / rowsTemperature;
    if (rowsTemperature_1 !=0)
        year_1Temperature = year_1Temperature / rowsTemperature_1;
    if (rowsTemperature_2 !=0)
        year_2Temperature = year_2Temperature / rowsTemperature_2;
    if (rowsTemperature_3 !=0)
        year_3Temperature = year_3Temperature / rowsTemperature_3;
    if (rowsTemperature_4 !=0)
        year_4Temperature = year_4Temperature / rowsTemperature_4;
}

void DataWindow::createFullnessChart(int tab_index)
{

    dailyFullnessInit();
    weeklyFullnessInit();
    monthlyFullnessInit();
    yearlyFullnessInit();

    switch (tab_index)
    {
    case 0:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");
           *set0 << today_6Fullness << today_5Fullness << today_4Fullness << today_3Fullness << today_2Fullness << today_1Fullness << todayFullness;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Fullness Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Day -6" << "Day -5" << "Day -4" << "Day -3" << "Day -2" << "Day -1" << "Today";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->daily_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->daily_tab->layout()->takeAt(0);
            delete item;
        }
        ui->daily_tab->addWidget(chartView);

        break;
    }
    case 1:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");

        *set0 << week_6Fullness << week_5Fullness << week_4Fullness << week_3Fullness
              << week_2Fullness << week_1Fullness << weekFullness;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Fullness Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week -6" << "Week -5" << "Week -4" << "Week -3" << "Week -2" << "Week -1" << "Week";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->weekly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->weekly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->weekly_tab->addWidget(chartView);

        break;
    }
    case 2:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");

        *set0 << month_11Fullness << month_10Fullness << month_9Fullness << month_8Fullness << month_7Fullness
              << month_6Fullness  << month_5Fullness  << month_4Fullness << month_3Fullness << month_2Fullness
              << month_1Fullness  << monthFullness;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Fullness Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Month -11" << "Month -10" << "Month -9" << "Month -8" << "Month -7" << "Month -6"
                   << "Month -5"  << "Month -4"  << "Month -3" << "Month -2" << "Month -1" << "Month";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->monthly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->monthly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->monthly_tab->addWidget(chartView);

        break;
    }
    case 3:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");

        *set0 << year_4Fullness << year_3Fullness << year_2Fullness << year_1Fullness << yearFullness;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Fullness Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Year 4" << "Year -3" << "Year -2" << "Year -1" << "Year";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->yearly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->yearly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->yearly_tab->addWidget(chartView);

        break;
    }
    default:
        break;
    }
}

void DataWindow::createWeightChart(int tab_index)
{
    dailyWeightInit();
    weeklyWeightInit();
    monthlyWeightInit();
    yearlyWeightInit();

    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set1 = new QtCharts::QBarSet("Weight");

        *set1 <<today_6Weight << today_5Weight << today_4Weight << today_3Weight
              << today_2Weight << today_1Weight << todayWeight;
;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set1);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Day -6" << "Day -5" << "Day -4" << "Day -3" << "Day -2" << "Day -1" << "Today";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->daily_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->daily_tab->layout()->takeAt(0);
            delete item;
        }
        ui->daily_tab->addWidget(chartView);

        break;
    }
    case 1:
    {
        QtCharts::QBarSet *set1 = new QtCharts::QBarSet("Weight");

        *set1 << week_6Weight << week_5Weight << week_4Weight << week_3Weight
              << week_2Weight << week_1Weight << weekWeight;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set1);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week -6" << "Week -5" << "Week -4" << "Week -3" << "Week -2" << "Week -1" << "Week";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->weekly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->weekly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->weekly_tab->addWidget(chartView);

        break;
    }
    case 2:
    {
        QtCharts::QBarSet *set1 = new QtCharts::QBarSet("Weight");

        *set1 << month_11Weight << month_10Weight << month_9Weight << month_8Weight << month_7Weight
              << month_6Weight  << month_5Weight  << month_4Weight << month_3Weight << month_2Weight
              << month_1Weight  << monthWeight;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set1);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Month -11" << "Month -10" << "Month -9" << "Month -8" << "Month -7" << "Month -6"
                   << "Month -5"  << "Month -4"  << "Month -3" << "Month -2" << "Month -1" << "Month";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->monthly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->monthly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->monthly_tab->addWidget(chartView);

        break;
    }
    case 3:
    {
        QtCharts::QBarSet *set1 = new QtCharts::QBarSet("Weight");

        *set1 << year_4Weight << year_3Weight << year_2Weight << year_1Weight << yearWeight;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set1);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Year 4" << "Year -3" << "Year -2" << "Year -1" << "Year";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->yearly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->yearly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->yearly_tab->addWidget(chartView);

        break;
    }
    default:
        break;
    }
}

void DataWindow::createHumidityChart(int tab_index)
{
    dailyHumidityInit();
    weeklyHumidityInit();
    monthlyHumidityInit();
    yearlyHumidityInit();

    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set2 = new QtCharts::QBarSet("Humidity Percentage");

        *set2 << today_6Humidity << today_5Humidity << today_4Humidity << today_3Humidity
              << today_2Humidity << today_1Humidity << todayHumidity;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set2);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Day -6" << "Day -5" << "Day -4" << "Day -3" << "Day -2" << "Day -1" << "Today";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->daily_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->daily_tab->layout()->takeAt(0);
            delete item;
        }
        ui->daily_tab->addWidget(chartView);

        break;
    }
    case 1:
    {
        QtCharts::QBarSet *set2 = new QtCharts::QBarSet("Humidity Percentage");

        *set2 << week_6Humidity << week_5Humidity << week_4Humidity << week_3Humidity
              << week_2Humidity << week_1Humidity << weekHumidity;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set2);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week -6" << "Week -5" << "Week -4" << "Week -3" << "Week -2" << "Week -1" << "Week";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->weekly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->weekly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->weekly_tab->addWidget(chartView);

        break;
    }
    case 2:
    {
        QtCharts::QBarSet *set2 = new QtCharts::QBarSet("Humidity Percentage");

        *set2 << month_11Humidity << month_10Humidity << month_9Humidity << month_8Humidity << month_7Humidity
              << month_6Humidity  << month_5Humidity  << month_4Humidity << month_3Humidity << month_2Humidity
              << month_1Humidity  << monthHumidity;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set2);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Month -11" << "Month -10" << "Month -9" << "Month -8" << "Month -7" << "Month -6"
                   << "Month -5"  << "Month -4"  << "Month -3" << "Month -2" << "Month -1" << "Month";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->monthly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->monthly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->monthly_tab->addWidget(chartView);

        break;
    }
    case 3:
    {
        QtCharts::QBarSet *set2 = new QtCharts::QBarSet("Humidity Percentage");

        *set2 << year_4Humidity << year_3Humidity << year_2Humidity << year_1Humidity << yearHumidity;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set2);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Year 4" << "Year -3" << "Year -2" << "Year -1" << "Year";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->yearly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->yearly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->yearly_tab->addWidget(chartView);

        break;
    }
    default:
        break;
    }
}

void DataWindow::createTemperatureChart(int tab_index)
{
    dailyTemperatureInit();
    weeklyTemperatureInit();
    monthlyTemperatureInit();
    yearlyTemperatureInit();

    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set3 = new QtCharts::QBarSet("Temperature");

        *set3 << today_6Temperature << today_5Temperature << today_4Temperature << today_3Temperature
              << today_2Temperature << today_1Temperature << todayTemperature;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set3);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Day -6" << "Day -5" << "Day -4" << "Day -3" << "Day -2" << "Day -1" << "Today";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->daily_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->daily_tab->layout()->takeAt(0);
            delete item;
        }
        ui->daily_tab->addWidget(chartView);

        break;
    }
    case 1:
    {
        QtCharts::QBarSet *set3 = new QtCharts::QBarSet("Temperature");

        *set3 << week_6Temperature << week_5Temperature << week_4Temperature << week_3Temperature
              << week_2Temperature << week_1Temperature << weekTemperature;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set3);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week -6" << "Week -5" << "Week -4" << "Week -3" << "Week -2" << "Week -1" << "Week";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->weekly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->weekly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->weekly_tab->addWidget(chartView);

        break;
    }
    case 2:
    {
        QtCharts::QBarSet *set3 = new QtCharts::QBarSet("Temperature");

        *set3 << month_11Temperature << month_10Temperature << month_9Temperature << month_8Temperature << month_7Temperature
              << month_6Temperature  << month_5Temperature  << month_4Temperature << month_3Temperature << month_2Temperature
              << month_1Temperature  << monthTemperature;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set3);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Month -11" << "Month -10" << "Month -9" << "Month -8" << "Month -7" << "Month -6"
                   << "Month -5"  << "Month -4"  << "Month -3" << "Month -2" << "Month -1" << "Month";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->monthly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->monthly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->monthly_tab->addWidget(chartView);

        break;
    }
    case 3:
    {
        QtCharts::QBarSet *set3 = new QtCharts::QBarSet("Temperature");

        *set3 << year_4Temperature << year_3Temperature << year_2Temperature << year_1Temperature << yearTemperature;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set3);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Year 4" << "Year -3" << "Year -2" << "Year -1" << "Year";
        QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();
        axis->append(categories);
        chart->createDefaultAxes();
        chart->setAxisX(axis, series);

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        while(ui->yearly_tab->layout()->count() > 0)
        {
            QLayoutItem *item = ui->yearly_tab->layout()->takeAt(0);
            delete item;
        }
        ui->yearly_tab->addWidget(chartView);

        break;
    }
    default:
        break;
    }
}

void DataWindow::on_datawindowPickupButton_clicked()
{
    emit openExtraPickup(userId);
}

void DataWindow::on_fullnessTitleButton_clicked()
{
    typeOfDataSelected = 0;
    on_tabWidget_currentChanged(0);
}

void DataWindow::on_weigthTitleButton_clicked()
{
    typeOfDataSelected = 1;
    on_tabWidget_currentChanged(0);
}

void DataWindow::on_humidityTitleButton_clicked()
{
    typeOfDataSelected = 2;
    on_tabWidget_currentChanged(0);
}

void DataWindow::on_nextPickUpTitleButton_clicked()
{

}

void DataWindow::on_temperatutureTitleButton_clicked()
{
    typeOfDataSelected = 3;
    on_tabWidget_currentChanged(0);
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

void DataWindow::on_tabWidget_currentChanged(int index)
{
    switch (typeOfDataSelected) {
    case 0:
        ui->tabWidget->setCurrentIndex(index);
        createFullnessChart(index);
        break;
    case 1:
        ui->tabWidget->setCurrentIndex(index);
        createWeightChart(index);
        break;
    case 2:
        ui->tabWidget->setCurrentIndex(index);
        createHumidityChart(index);
        break;
    case 3:
        ui->tabWidget->setCurrentIndex(index);
        createTemperatureChart(index);
        break;
    default:
        break;
    }
}

void DataWindow::on_temperatureTitleButton_clicked()
{
    on_temperatureDataButton_clicked();
}
