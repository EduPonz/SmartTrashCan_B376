#ifndef DATAWINDOW_H
#define DATAWINDOW_H

#include <QWidget>
#include <QMainWindow>


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

private:
    Ui::DataWindow *ui;
    int userId;
};

#endif // DATAWINDOW_H
