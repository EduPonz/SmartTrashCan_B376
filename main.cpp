/*! \mainpage Application Introduction
 *
 * \section intro_sec Introduction
 *
 * The software developed for this project is
 * an object oriented programming Qt cross-platform
 * desktop application in which the users
 * can create an account that provides access
 * to information specific to their domestic trash.
 *  In this account, the users can update their profile,
 * which contains personal information such as address,
 *  email, or payment details. It displays current
 * data about the fullness, weight, temperature and
 * humidity level of the trash container. It also shows the
 * date of the next pickup, allows the user to
 * order extra pickups services and provides them
 * access to their order history. All the information collected
 * by the application is stored in SQLite databases, which are
 * then used to create and display charts.
 *
 * */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
