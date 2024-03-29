#include "optionswindow.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "userdatabasemanager.h"
#include "historydatabasemanager.h"
#include "ui_optionswindow.h"
#include <QDebug>
#include <QSqlQuery>
#include <QHeaderView>

OptionsWindow::OptionsWindow(QWidget *parent, int id) : QWidget(parent), ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    userId = id;
    QString idString;
    qDebug() << "OptionsWindow::OptionsWindow - ID: " << idString.number(userId);
    populate();
    populateTable();
}

void OptionsWindow::populateTable(){
    QSqlQuery query = historyManager.rowNumberRetrieve(userId);
    query = historyManager.historyDatabaseRetrieve(userId);

    ui->historyTable->setRowCount(historyManager.rows); ui->historyTable->setColumnCount(6);
    ui->historyTable->horizontalHeader()->setStretchLastSection(true);
    ui->historyTable->horizontalHeader()->setFixedHeight(35);
    ui->historyTable->horizontalHeader()->setAlternatingRowColors(true);

    //ui->historyTable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    ui->historyTable->horizontalHeader()->resizeSection( 1, 150 );
    ui->historyTable->horizontalHeader()->resizeSection( 2, 150 );
    ui->historyTable->horizontalHeader()->resizeSection( 3, 150 );
    ui->historyTable->horizontalHeader()->resizeSection( 4, 150 );

    int localeRows = historyManager.rows;

    query.first();
    for (int i = 0; i < localeRows; ++i) {
        ui->historyTable->setItem(i, 0, new QTableWidgetItem(query.value(12).toString()));
        ui->historyTable->setItem(i, 1, new QTableWidgetItem(query.value(3).toString()));
        ui->historyTable->setItem(i, 2, new QTableWidgetItem(query.value(4).toString()));
        ui->historyTable->setItem(i, 3, new QTableWidgetItem(query.value(5).toString()));
        ui->historyTable->setItem(i, 4, new QTableWidgetItem(query.value(11).toString()));
        ui->historyTable->setItem(i, 5, new QTableWidgetItem(query.value(10).toString()));
        if (!query.next())
            break;
    }

    //ui->historyTable->setItem(0, 1, new QTableWidgetItem());
}

void OptionsWindow::populate()
{
    optionsQuery = optionsDatabaseManager.userDataBaseRetrieve(userId);
    if (optionsQuery.isActive()){
        optionsQuery.first();
        ui->optionsUsernameLineEdit->setText(optionsQuery.value(0).toString());
        ui->optionsPasswordLineEdit->setText(optionsQuery.value(1).toString());
        ui->optionsFullNameLineEdit->setText(optionsQuery.value(2).toString());
        ui->optionsAddressLineEdit->setText(optionsQuery.value(3).toString());
        ui->optionsPhoneNumberLineEdit->setText(optionsQuery.value(4).toString());
        ui->optionsEmailLineEdit->setText(optionsQuery.value(5).toString());
        ui->optionsCardNumberLineEdit->setText(optionsQuery.value(7).toString());
        ui->optionsCvvLineEdit->setText(optionsQuery.value(8).toString());
        ui->optionsExpirationDateLineEdit->setText(optionsQuery.value(9).toString());
        if (!optionsQuery.value(6).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_VISA)){
            ui->optionsVisaCheckBox->setChecked(1);
        }else if (!optionsQuery.value(6).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_MASTERCARD)){
            ui->optionsMastercardCheckBox->setChecked(1);
        }else if (!optionsQuery.value(6).toString().compare(optionsDatabaseManager.PAYMENT_METHOD_BOTTLECAPS)){
            ui->optionsBottlecapsCheckBox->setChecked(1);
        }else{
            ui->optionsVisaCheckBox->setChecked(0);
            ui->optionsMastercardCheckBox->setChecked(0);
            ui->optionsBottlecapsCheckBox->setChecked(0);
        }
    }
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}

void OptionsWindow::on_optionsApplyChangesButton_clicked()
{
    QString newUserName = ui->optionsUsernameLineEdit->text();
    QString password = ui->optionsPasswordLineEdit->text();
    QString full_name = ui->optionsFullNameLineEdit->text();
    QString address =  ui->optionsAddressLineEdit->text();
    QString phone_number = ui->optionsPhoneNumberLineEdit->text();
    QString email = ui->optionsEmailLineEdit->text();
    QString payment_method;
    QString card_number = ui->optionsCardNumberLineEdit->text();
    QString cvv = ui->optionsCvvLineEdit->text();
    QString expiry_date = ui->optionsExpirationDateLineEdit->text();

    if (ui->optionsVisaCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_VISA;
    }else if (ui->optionsMastercardCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_MASTERCARD;
    }else if (ui->optionsBottlecapsCheckBox->checkState()){
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_BOTTLECAPS;
    }else {
        payment_method = optionsDatabaseManager.PAYMENT_METHOD_UNKNOWN;
    }

    if ((!newUserName.isEmpty()) && (!password.isEmpty()) && (!full_name.isEmpty()) && (!address.isEmpty())){
        QString idString;
        qDebug() << "OptionsWindow::on_optionsApplyChangesButton_clicked - ID: " << idString.number(userId);
        if (optionsDatabaseManager.availableUpdateUserName(userId, newUserName)){
            optionsDatabaseManager.userDataBaseUpdate(userId, newUserName, password,
                                                      full_name, address, phone_number, email,
                                                      payment_method, card_number, cvv, expiry_date);
            emit apply_changes(full_name);
            ui->optionsOutPutLabel->setText("Your user has been updated");
        }else{
            ui->optionsOutPutLabel->setText("Username not avilable. Try another one");
        }
    }else{
            ui->optionsOutPutLabel->setText("Some fields are missing");
    }
}

void OptionsWindow::on_optionsDeleteUserButton_clicked()
{
    if (optionsDatabaseManager.userDatabaseDeleteUser(userId)){
        emit user_deleted();
    }else{
        ui->optionsOutPutLabel->setText("The user could not be deleted");
    }
}
