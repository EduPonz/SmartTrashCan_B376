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

private:
    Ui::DataWindow *ui;
    int userId;
};

#endif // DATAWINDOW_H
