#include "datawindow.h"
#include "ui_datawindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>
#include <QString>

DataWindow::DataWindow(QWidget *parent, int id) : QWidget(parent), ui(new Ui::DataWindow)
{
    userId = id;
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    createWeightChart(0);
}

DataWindow::~DataWindow()
{
    delete ui;
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
        chart->setTitle("Simple barchart example");
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

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Simple barchart example");
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

        *set0 << 1 << 2 << 3 << 4 << 5 << 6 << 7;

        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        series->append(set0);

        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->addSeries(series);
        chart->setTitle("Simple barchart example");
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

void DataWindow::on_tabWidget_currentChanged(int index)
{
        createWeightChart(index);
        qDebug() << "DataWindow::on_tabWidget_currentChanged - tab selected" << QString::number(index);
}
