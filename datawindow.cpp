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
    trashInfoManager.fakeTrashInfo(userId);
    typeOfDataSelected = 0;
    createFullnessChart(0);
}

DataWindow::~DataWindow()
{
    delete ui;
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
    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Weight");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Weight");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week 1" << "Week 2" << "Week 3" << "Week 4" << "Week 5" << "Week 6" << "Week 7";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Weight");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "January" << "February" << "March" << "April" << "May" << "June" << "July"
                   << "August" << "September" << "October" << "November" << "December";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Weight");

        *set0 << 1 << 2 << 3 << 4 << 5;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Weight");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "2017" << "2018" << "2019" << "2020" << "2021";
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
    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Humidity Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Humidity Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week 1" << "Week 2" << "Week 3" << "Week 4" << "Week 5" << "Week 6" << "Week 7";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Humidity Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "January" << "February" << "March" << "April" << "May" << "June" << "July"
                   << "August" << "September" << "October" << "November" << "December";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Humidity Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Humidity Percentage");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "2017" << "2018" << "2019" << "2020" << "2021";
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
    switch (tab_index) {
    case 0:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Temperature");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Daily Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Temperature");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "Week 1" << "Week 2" << "Week 3" << "Week 4" << "Week 5" << "Week 6" << "Week 7";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Temperature");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "January" << "February" << "March" << "April" << "May" << "June" << "July"
                   << "August" << "September" << "October" << "November" << "December";
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Temperature");

        *set0 << 1 << 2 << 3 << 4 << 5;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Temperature");
        chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

        QStringList categories;
        categories << "2017" << "2018" << "2019" << "2020" << "2021";
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
