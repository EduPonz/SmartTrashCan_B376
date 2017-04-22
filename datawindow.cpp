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
    dateStringsInit();
    createFullnessChart(0);
}

DataWindow::~DataWindow()
{
    delete ui;
}

void DataWindow::dateStringsInit()
{
    todayString = QDateTime::fromString(QDate::currentDate().toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_1String = QDateTime::fromString(QDate::currentDate().addDays(-1).toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_2String = QDateTime::fromString(QDate::currentDate().addDays(-2).toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_3String = QDateTime::fromString(QDate::currentDate().addDays(-3).toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_4String = QDateTime::fromString(QDate::currentDate().addDays(-4).toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_5String = QDateTime::fromString(QDate::currentDate().addDays(-5).toString(Qt::ISODate), Qt::ISODate).date().toString();
    today_6String = QDateTime::fromString(QDate::currentDate().addDays(-6).toString(Qt::ISODate), Qt::ISODate).date().toString();
}

void DataWindow::dailyFullnessInit()
{
    QSqlQuery query = trashInfoManager.trashInfoDatabaseRetrieveDaily(userId);
    query.first();
    todayFulness = 0;
    today_1Fulness = 0;
    today_2Fulness = 0;
    today_3Fulness = 0;
    today_4Fulness = 0;
    today_5Fulness = 0;
    today_6Fulness = 0;
    int rowsFullness = 0;
    int rowsFullness_1 = 0;
    int rowsFullness_2 = 0;
    int rowsFullness_3 = 0;
    int rowsFullness_4 = 0;
    int rowsFullness_5 = 0;
    int rowsFullness_6 = 0;

    do {
        QString databaseDate = QDateTime::fromString(query.value(0).toString(), Qt::ISODate).date().toString();
        if (!databaseDate.compare(todayString))
        {
            todayFulness += query.value(1).toFloat();
            rowsFullness ++;
        }
        if (!databaseDate.compare(today_1String))
        {
            today_1Fulness += query.value(1).toFloat();
            rowsFullness_1 ++;
        }
        if (!databaseDate.compare(today_2String))
        {
            today_2Fulness += query.value(1).toFloat();
            rowsFullness_2 ++;
        }
        if (!databaseDate.compare(today_3String))
        {
            today_3Fulness += query.value(1).toFloat();
            rowsFullness_3 ++;
        }
        if (!databaseDate.compare(today_4String))
        {
            today_4Fulness += query.value(1).toFloat();
            rowsFullness_4 ++;
        }
        if (!databaseDate.compare(today_5String))
        {
            today_5Fulness += query.value(1).toFloat();
            rowsFullness_5 ++;
        }
        if (!databaseDate.compare(today_6String))
        {
            today_6Fulness += query.value(1).toFloat();
            rowsFullness_6 ++;
        }
    }while (query.next());

    if (rowsFullness != 0)
        todayFulness = todayFulness / rowsFullness;
    if (rowsFullness_1 !=0)
        today_1Fulness = today_1Fulness / rowsFullness_1;
    if (rowsFullness_2 !=0)
        today_2Fulness = today_2Fulness / rowsFullness_2;
    if (rowsFullness_3 !=0)
        today_3Fulness = today_3Fulness / rowsFullness_3;
    if (rowsFullness_4 !=0)
        today_4Fulness = today_4Fulness / rowsFullness_4;
    if (rowsFullness_5 !=0)
        today_5Fulness = today_5Fulness / rowsFullness_5;
    if (rowsFullness_6 !=0)
        today_6Fulness = today_6Fulness / rowsFullness_6;
}

void DataWindow::createFullnessChart(int tab_index)
{
    dailyFullnessInit();

    switch (tab_index)
    {
    case 0:
    {
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");
           *set0 << today_6Fulness << today_5Fulness << today_4Fulness << today_3Fulness << today_2Fulness << today_1Fulness << todayFulness;

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

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Weekly Fullness Percentage");
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Monthly Fullness Percentage");
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
        QtCharts::QBarSet *set0 = new QtCharts::QBarSet("Fullness Percentage");

        *set0 << 1 << 2 << 3 << 4 << 5;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Yearly Fullness Percentage");
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
